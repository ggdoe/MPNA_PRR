#include "tools.h"

int LWORK;
double *_tmp_mm; // size m*m
double *_tmp_lwork; // size > n*m
void *_tmp_m; // ipiv ou vp_i   (size m)

static void init_prr(int n, int m, struct projection *p, struct spectre *spectre, double *A, int *max_it);
static void free_prr(struct projection *p);

struct spectre prr(int n, int m, double *restrict A, double *restrict x, struct prr_info *restrict prrinfo, int max_it, double _epsilon)
{
	struct projection p;
	struct spectre spectre;
	double *residu;
	double maxres, time_start, time_end;
	int count = 0;

	init_prr(n,m, &p, &spectre, A, &max_it);

	normalize(x,n);

	time_start = omp_get_wtime();
	// boucle des itérations de l'algorithme PRR
	do
	{
		// algorithme PRR:
		projection(&p, A, n, m, x); // O(m * n²)
		resolution_sev(&spectre, &p,m); // O(m³)
		retour_espace_depart(n, m, p.Vm, &spectre); // O(n * m²)
		residu = calcul_residu(n, m, A, &spectre); // O(m * n²)

		maxres = max(residu,m);
		// printf("max : %le\n", max(residu,m));

		// nouveau vecteur initial :
		get_new_x(n,m,x,residu,spectre.vec_p); // O(m * n)

		if(++count >= max_it)
			break;
	}while(maxres > _epsilon);
	time_end = omp_get_wtime();

	free_prr(&p);
	prrinfo->max_residu = maxres;
	prrinfo->nb_it = count;
	prrinfo->tps_exec = time_end - time_start;
	prrinfo->got_result = 1; // TODO (eventuellement) à virer à l'aide de define
	
	return spectre;
}

#ifdef MULTIPRR
struct spectre multi_prr(int n, int m, double *restrict A, double *restrict x, struct prr_info *restrict prrinfo, int max_it, double _epsilon, int interval_comm)
{
	struct projection p;
	struct spectre spectre;
	double *residu;
	double maxres, time_start, time_end;
	double minmaxres; // min des résidus max des process MPI
	int count = 0;

	init_prr(n,m, &p, &spectre, A, &max_it);

	normalize(x,n);

	time_start = omp_get_wtime();
	// boucle des itérations de l'algorithme PRR
	for(;;){
		// algorithme PRR:
		projection(&p, A, n, m, x); // O(m * n²)
		resolution_sev(&spectre, &p,m); // O(m³)
		retour_espace_depart(n, m, p.Vm, &spectre); // O(n * m²)
		residu = calcul_residu(n, m, A, &spectre); // O(m * n²)

		maxres = max(residu,m);
		// printf("max : %le\n", max(residu,m));

		// nouveau vecteur initial :
		get_new_x(n,m,x,residu,spectre.vec_p); // O(m * n)
		///

		// On évite de lancer les comm mpi à chaque itération
		count++;
		if (maxres < _epsilon || count >= max_it){
			MPI_Allreduce(&maxres, &minmaxres, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
			break;
		}
		else if(count % interval_comm == 0){
			MPI_Allreduce(&maxres, &minmaxres, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
			if(minmaxres < _epsilon)
				break;
		}
	}
	time_end = omp_get_wtime();

	free_prr(&p);
	prrinfo->max_residu = maxres;
	prrinfo->nb_it = count;
	prrinfo->tps_exec = time_end - time_start;
	prrinfo->got_result = (minmaxres == maxres);
	
	return spectre;
}
#endif

/////////

static void init_prr(int n, int m, struct projection *restrict p, struct spectre *restrict spectre, double *A, int *max_it)
{
	LWORK = n * (m < 4 ? 4 : m); // 4*n   ou   n*m
	_tmp_mm = malloc(m*m * sizeof(double));
	_tmp_lwork = malloc(LWORK * sizeof(double));
	_tmp_m = malloc(m * sizeof(double));
	
	p->B1 = malloc(m*m*sizeof(double));
	p->B2 = malloc(m*m*sizeof(double));
	p->Vm = malloc(LWORK*sizeof(double));

	spectre->vp = malloc(m * sizeof(double));
	spectre->vec_p = malloc(LWORK * sizeof(double)); // pour une raison inconue le programme crash (m=3) si l'on met n*m
	
	if(m > n){
		printf("'m' doit etre plus petit ou egal a 'n'.\n");
		exit(1);
	}
	if(*max_it <= 0)
		*max_it = __INT32_MAX__;
	// vérification que la matrice A est symétrique, sinon l'algo ne s'applique pas
	#pragma omp parallel for
	for(int i = 0; i < n; i++)
		for(int j = i+1; j < n; j++)
			if(A[i * n + j] != A[j * n + i]){
				printf("'A' doit etre symmetrique.\n");
				exit(1);
			}
}

static void free_prr(struct projection *p)
{
	free(_tmp_mm);
	free(_tmp_lwork);
	free(_tmp_m);
	
	free(p->B1);
	free(p->B2);
	free(p->Vm);
}

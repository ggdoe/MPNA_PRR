#include "tools.h"

int LWORK;
double *_tmp_mm; // taille  m*m
double *_tmp_lwork; // taille > n*m
void *_tmp_m; // ipiv ou vp_i   (taille m)

static void init_prr(int n, int m, struct projection *p, struct spectre *spectre, double *A, int *max_it);
static void free_prr(struct projection *p);

#ifndef MULTIPRR
struct spectre prr(int n, double *restrict A, double *restrict x, struct prr_info *restrict prrinfo, struct prgm_config *restrict config)
{
	struct projection p;
	struct spectre spectre;
	double *residu;
	double maxres;
	int count = 0;
	struct timespec time_start, time_end;

	int m = config->m;
	int max_it = config->max_it;
	double _epsilon = config->epsilon;

	init_prr(n,m, &p, &spectre, A, &max_it);
	normalize(x,n);
	
	clock_gettime(CLOCK_MONOTONIC_RAW, &time_start);

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
	clock_gettime(CLOCK_MONOTONIC_RAW, &time_end);

	free_prr(&p);
	prrinfo->max_residu = maxres;
	prrinfo->nb_it = count;
	prrinfo->tps_exec = (double)(time_end.tv_sec - time_start.tv_sec) * 1e6 + (double)(time_end.tv_nsec - time_start.tv_nsec) * 1e-3;
	prrinfo->got_result = 1;
	
	return spectre;
}

#else

struct spectre multi_prr(int n, double *restrict A, double *restrict x, struct prr_info *restrict prrinfo, struct prgm_config *restrict config)
{
	struct projection p;
	struct spectre spectre;
	double *residu;
	double maxres;
	double minmaxres; // min des résidus max des process MPI
	int count = 0;
	struct timespec time_start, time_end;

	int m = config->m;
	int max_it = config->max_it;
	int interval_comm = config->freq;
	double _epsilon = config->epsilon;

	init_prr(n,m, &p, &spectre, A, &max_it);
	normalize(x,n);

	clock_gettime(CLOCK_MONOTONIC_RAW, &time_start);
	
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

		// On évite de lancer les communications mpi à chaque itération
		count++;
		if (maxres < _epsilon || count >= max_it){
			clock_gettime(CLOCK_MONOTONIC_RAW, &time_end);
			MPI_Allreduce(&maxres, &minmaxres, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
			break;
		}
		else if(count % interval_comm == 0){
			MPI_Allreduce(&maxres, &minmaxres, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
			if(minmaxres < _epsilon){
				clock_gettime(CLOCK_MONOTONIC_RAW, &time_end);
				break;
			}
		}
	}

	free_prr(&p);
	prrinfo->max_residu = maxres;
	prrinfo->nb_it = count; // temps exec en µs
	prrinfo->tps_exec = (double)(time_end.tv_sec - time_start.tv_sec) * 1e6 + (double)(time_end.tv_nsec - time_start.tv_nsec) * 1e-3;
	prrinfo->got_result = (minmaxres == maxres);
	
	return spectre;
}
#endif

/////////

static void init_prr(int n, int m, struct projection *restrict p, struct spectre *restrict spectre, double *A, int *max_it)
{
	LWORK = n * (m < 4 ? 4 : m); // 4*n ou n*m
	_tmp_mm = MALLOC(m*m * sizeof(double));
	_tmp_lwork = MALLOC(LWORK * sizeof(double));
	_tmp_m = MALLOC(m * sizeof(double));
	
	p->B1 = MALLOC(m*m*sizeof(double));
	p->B2 = MALLOC(m*m*sizeof(double));
	p->Vm = MALLOC(LWORK*sizeof(double));

	spectre->vp = MALLOC(m * sizeof(double));
	spectre->vec_p = MALLOC(LWORK * sizeof(double));
	
	if(m > n){
		printf("'m' doit etre plus petit ou egal a 'n'.\n");
		exit(1);
	}
	if(*max_it <= 0)
		*max_it = __INT32_MAX__;
	// vérification que la matrice A est symétrique, sinon l'algorithme ne s'applique pas
	// #pragma omp parallel for
	for(int i = 0; i < n; i++)
		for(int j = i+1; j < n; j++)
			if(A[i * n + j] != A[j * n + i]){
				printf("'A' doit etre symmetrique.\n");
				exit(1);
			}
}

static void free_prr(struct projection *p)
{
	FREE(_tmp_mm);
	FREE(_tmp_lwork);
	FREE(_tmp_m);
	
	FREE(p->B1);
	FREE(p->B2);
	FREE(p->Vm);
}

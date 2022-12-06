#include "tools.h"
#include <mpi.h>

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
	double maxres;
	int count = 0;

	init_prr(n,m, &p, &spectre, A, &max_it);

	normalize(x,n);

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

	free_prr(&p);
	if(prrinfo != NULL){
		prrinfo->max_residu = maxres;
		prrinfo->nb_it = count;
	}
	
	return spectre;
}

struct spectre multi_prr(int *argc, char*** argv, int n, int m, double *restrict A, double *restrict x, struct prr_info *restrict prrinfo, int max_it, double _epsilon)
{
	struct spectre spectre;
	int nb_mpi, rank_mpi;
	MPI_Init(argc, argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nb_mpi);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank_mpi);
	srand48(rank_mpi);

	printf("Nombre de processus = %d\n", nb_mpi);
	spectre = prr(n, m, A, x, prrinfo, max_it, _epsilon);

	///// TODO
	//// recopier prr + comm mpi
	/////
	
	print_separator("vp");
	print_matrice(spectre.vp, 1, m);

	print_separator("vecteur ritz");
	print_matrice(spectre.vec_p, m, n);

	printf("count : %d\n", prrinfo->nb_it);
	printf("max residu : %lg\n", prrinfo->max_residu);

	MPI_Finalize();
}

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
	free(_tmp_mm);
	free(_tmp_lwork);
	free(_tmp_m);
	
	free(p->B1);
	free(p->B2);
	free(p->Vm);
}

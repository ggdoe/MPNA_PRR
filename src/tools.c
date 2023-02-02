#include "tools.h"

static double * read_matricetxt(char *filename, int *n, int *m);
static double * read_matrixmarket(char *filename, int *n, int *m);

// row major (n,m)
void print_matrice(double *A, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			printf("%.3lf\t", A[m*i + j]);
		printf("\n");
	}
}

void rand_vector(int n, double *x)
{
	for(int i = 0; i < n; i++)
		x[i] = 2 * drand48() - 1.;
}

// A = randi(n,n); A = A * transpose(A); A = A / median(median(A)); [v1, v2] = eig(A); A,v1,v2

// row major (n,m)
double * read_matrice(char *filename, int *n, int *m)
{
	if(!strcmp(strrchr(filename, '.'), ".txt"))
		return read_matricetxt(filename, n, m);
	else if(!strcmp(strrchr(filename, '.'), ".mtx"))
		return read_matrixmarket(filename, n, m);
	else {
		fprintf(stderr, "Erreur extention de fichier non reconnue. (%s)\n", filename);
		exit(EXIT_FAILURE);
	}
}

static double * read_matricetxt(char *filename, int *n, int *m)
{
	int N, M;
	FILE *fd = fopen(filename, "r");

	if( fd == NULL )
	{
		fprintf(stderr, "Erreur lors de l'exécution de fopen(). Il est probable que le fichier %s ne soit pas présent dans le dossier courant.\n", filename);
		exit(EXIT_FAILURE);
	}

	fscanf(fd, "%d", &N);
	fscanf(fd, "%d", &M);

	double *A = MALLOC(N * M * sizeof(double));

	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			fscanf(fd, "%lf" , &A[ M * i + j]);

	fclose(fd);
	if(n)
		*n = N;
	if(m)
		*m = M;
	return A;
}

static double * read_matrixmarket(char *filename, int *n, int *m)
{
	int N, M, nblines;
	FILE *fd = fopen(filename, "r");

	if( fd == NULL )
	{
		fprintf(stderr, "Erreur lors de l'exécution de fopen(). Il est probable que le fichier %s ne soit pas présent dans le dossier courant.\n", filename);
		exit(EXIT_FAILURE);
	}

	fscanf(fd, "%d", &N);
	fscanf(fd, "%d", &M);
	fscanf(fd, "%d", &nblines);

	double *A = MALLOC(N * M * sizeof(double));

	for (int i = 0; i < N * M; i++)
		A[i] = 0;

	int i,j;
	double value;
	for (int l = 0; l < nblines; l++){
		fscanf(fd, "%d", &i);	
		fscanf(fd, "%d", &j);
		fscanf(fd, "%lf", &value);
		--i;--j;
		A[i * M + j] = value;
		A[j * M + i] = value;
	}

	fclose(fd);
	if(n)
		*n = N;
	if(m)
		*m = M;
	return A;
}

void normalize(double *x, int n)
{
	double inv_norm = 1/sqrt(DDOT(n, x, 1, x, 1));
	DSCAL(n, inv_norm, x, 1);
}

double max(double *x, int n)
{
	double max = 0.;
	for(int i = 0; i < n; i++)
		if(x[i] > max)
			max = x[i];

	return max;
}

FILE* __result;

void log_result(struct prr_info prrinfo)
{
	fprintf(__result, "%d %lf %lg\n", prrinfo.nb_it, prrinfo.tps_exec, prrinfo.max_residu);
}

void init_log()
{
	__result = fopen("result.dat", "a");
}	

void log_config(struct prgm_config* config, int n)
{
	int nb_mpi, nb_omp;

	#ifdef MKL
		nb_omp = mkl_get_max_threads();
		#define STR_COMPILER "icc"
	#else
		nb_omp = openblas_get_num_threads();
		// nb_omp = omp_get_max_threads();
		#define STR_COMPILER "gcc"
	#endif

	#ifdef MULTIPRR
		MPI_Comm_size(MPI_COMM_WORLD, &nb_mpi);
	#else
		nb_mpi = 1;
	#endif

	FILE* config_fd = fopen("config.dat", "w");
	fprintf(config_fd, 
		STR_COMPILER "\tmpi=%d\tomp=%d\tfilename=%s\tn=%d\tm=%d\t"\
		"epsilon=%lg\tnb_rep=%d\tfreq=%d\tmax_it=%d\n", 
		nb_mpi, nb_omp,
		config->filename, n, config->m, 
		config->epsilon, config->nb_rep, 
		config->freq, config->max_it);
	fclose(config_fd);
	#undef STR_COMPILER
}

void close_result()
{
	fclose(__result);
}


#include "tools.h"

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
	fprintf(__result, "%d %lf %g\n", prrinfo.nb_it, prrinfo.tps_exec, prrinfo.max_residu);
}

void init_log()
{
	__result = fopen("result.dat", "a");
}	

void log_config(struct prgm_config* config, int n)
{
	FILE* config_fd = fopen("config.dat", "w");
	fprintf(config_fd, "filename=%s\tepsilon=%lg\tfreq=%d\tn=%d\tm=%d\tmax_it=%d\tnb_rep=%d\n", config->filename, config->epsilon, config->freq, n, config->m, config->max_it, config->nb_rep);
	fclose(config_fd);
}

void close_result()
{
	fclose(__result);
}


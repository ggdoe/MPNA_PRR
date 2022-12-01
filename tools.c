#include "tools.h"

// row major (n,m)
void print_matrice(double *A, int n, int m)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			printf("%lf " ,A[m*i + j]);
		printf("\n");
	}
}

// A = randi(n,n); A = A * transpose(A); A = A / median(median(A)); [v1, v2] = eig(A); A,v1,v2

// row major (n,m)
double * read_matrice(char *filename, int *n, int *m)
{
	int N, M;
	FILE *fd = fopen(filename, "r");

	fscanf(fd, "%d", &N);
	fscanf(fd, "%d", &M);

	double *A = malloc(N * M * sizeof(double));

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
	double inv_norm = 1/sqrt(cblas_ddot(n, x, 1, x, 1));
	cblas_dscal(n, inv_norm, x, 1);
}

// void std_to_sym(double *in, double *out, int m)
// {
// 	size_t shift = 0;

// 	for (size_t i = 0; i < m; i++) {
// 		memcpy(out + shift, in + i * m + i, (m - i) * sizeof(double));
// 		shift += m - i;
// 	}
// }

// void sym_to_std(double *in, double *out, int m)
// {
// 	size_t shift = 0;

// 	for (size_t i = 0; i < m; i++) {
// 		memcpy(out + i * m + i, in + shift, (m - i) * sizeof(double));
// 		shift += m - i;
// 		for (size_t j = i + 1; j < m; j++)
// 			out[j * m + i] = out[i * m + j];
// 	}
// }


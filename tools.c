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

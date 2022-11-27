#include "tools.h"
#include <cblas.h>
#include <lapacke.h>

int main()
{
	double a[] = {1,2,3,4};
	double b[] = {4,1,1,2};
	double c[] = {0,0,0,0};

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
		2, 2, 2, 1., a, 2, b, 2, 0, c, 2);

	int n, m;
	//double *A = 
	double *A = read_matrice("mat1.txt", &n, &m);
	print_matrice(c, 2, 2);
	// print_matrice(A, 20, 20);

	free(A);

	return 0;
}


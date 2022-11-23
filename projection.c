#include <cblas.h>
#include "tools.h"
#include "math.h"

struct B_proj {
	double *Bm;
	double *Bm1;
};

// A : taille n*n
// B : taille m*m
void* projection(double *A, int n, int m, const double* x)
{
	struct B_proj B;
	B.Bm = malloc(m*m*sizeof(double));
	B.Bm1 = malloc(m*m*sizeof(double));

	double* c = malloc(2*m*sizeof(double));
	double* Aj_x = malloc(m*sizeof(double));

	for(int j = 0; j < 2*m-1; j++)
	{
		cblas_dgemv(CblasRowMajor, CblasNoTrans, m, n, 1, A, m, x, 1, 1, Aj_x, 1); //Aj_x = A^j *x
		c[j] = cblas_ddot(n, x, 1, Aj_x, 1);

		printf("Aj_x\n");
		print_matrice(Aj_x, 1, m);
	}

	printf("c à la fin\n");
	print_matrice(c, 1, m);


	//Remplir B

	free(c);
	free(Aj_x);
}


int main()
{
	int n = 3, m = 3;
	double *A, *x;
	A = malloc(n*n*sizeof(double));
	x = malloc(n*sizeof(double));

	A = read_matrice("mat2.txt", &n, &m);
	printf("A\n");
	print_matrice(A, m, m);

	x[0] = 4.0; x[1] = 5.0; x[2] = 6.0;
	x[0] = (x[0]/sqrt(cblas_ddot(n, x, 1, x, 1))); x[1] = (x[1]/sqrt(cblas_ddot(n, x, 1, x, 1))); x[2] = (x[2]/sqrt(cblas_ddot(n, x, 1, x, 1)));

	printf("x normé\n");
	print_matrice(x, 1, m);


	projection(A, n, m, x);

	free(A);
	free(x);

	return 0;
}


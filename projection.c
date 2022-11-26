#include <cblas.h>
#include <string.h>
#include "tools.h"
#include "math.h"

//Commande : gcc projection.c tools.c -lm -lcblas -o projection_test.out && ./projection_test.out

struct B_proj {
	double *Bm;
	double *Bm1;
};

double* Vm;

// A : taille n*n
// B : taille m*m
void* projection(double *A, int n, int m, const double* x)
{
	struct B_proj B;
	B.Bm = malloc(m*m*sizeof(double));
	B.Bm1 = malloc(m*m*sizeof(double));

	double* c = malloc(2*m*sizeof(double));
	double* Aj_x = malloc(m*sizeof(double));

	//Remplir c et V_m
	memcpy(Vm, x, m*sizeof(double*));
	for(int j = 0; j < 2*m; j++)
	{
		cblas_dgemv(CblasRowMajor, CblasNoTrans, m, n, 1, A, m, x, 1, 1, Aj_x, 1); //Aj_x = A^j *x

		if(j < m) //V_m est de taille m
		{
			memcpy(Vm+(j+1)*m, Aj_x, m*sizeof(double));
		}

		c[j] = cblas_ddot(m, x, 1, Aj_x, 1); //c_ij = x* A^j *x
	}

	//Remplir Bm et Bm-1
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			B.Bm1[i*m + j] = c[i+j];
			B.Bm[i*m + j] = c[i+j+1];
		}
	}

	//printf("c à la fin\n"); print_matrice(c, 1, 2*m);
	printf("Bm-1\n"); print_matrice(B.Bm1, m, m);
	printf("Bm\n"); print_matrice(B.Bm, m, m);
	printf("Vm\n"); print_matrice(Vm, m, m);

	free(c);
	free(Aj_x);
	free(B.Bm);
	free(B.Bm1);
}


int main()
{
	int n = 3, m = 3;
	double *A, *x;
	A = malloc(n*n*sizeof(double));
	x = malloc(n*sizeof(double));
	Vm = malloc(m*m*sizeof(double));

	A = read_matrice("mat2.txt", &n, &m);
	//printf("A\n"); print_matrice(A, m, m);

	x[0] = 4.0; x[1] = 5.0; x[2] = 6.0;
	x[0] = (x[0]/sqrt(cblas_ddot(n, x, 1, x, 1))); x[1] = (x[1]/sqrt(cblas_ddot(n, x, 1, x, 1))); x[2] = (x[2]/sqrt(cblas_ddot(n, x, 1, x, 1)));

	printf("x normé\n"); print_matrice(x, 1, m);

	projection(A, n, m, x);

	free(A);
	free(x);
	free(Vm);

	return 0;
}


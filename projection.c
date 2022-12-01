#include "tools.h"

//Commande : gcc projection.c tools.c -lm -lcblas -o projection_test.out && ./projection_test.out

struct B_proj {
	double *B1;
	double *B2;
};

double* Vm;

// A : taille n*n
// B : taille m*m
void* projection(double *A, int n, int m, const double* x)
{
	struct B_proj B;
	B.B1 = malloc(m*m*sizeof(double));
	B.B2 = malloc(m*m*sizeof(double));

	double* c = malloc(2*m*sizeof(double));
	double* Aj_x = malloc(m*sizeof(double));
	double* tmp = malloc(m*sizeof(double));

	//Remplir c et V_m
	memcpy(Vm, x, m*sizeof(double*));
	memcpy(Aj_x, x, m*sizeof(double*));
	c[0] = 1.;
	for(int j = 1; j < 2*m; j++)
	{
		cblas_dgemv(CblasRowMajor, CblasNoTrans, 
			m, m, 1, A, m, Aj_x, 1, 0, tmp, 1); //Aj_x = A^j *x
		memcpy(Aj_x, tmp, m*sizeof(double*));

		if(j < m) //V_m est de taille m
			memcpy(Vm+j*m, Aj_x, m*sizeof(double));

		c[j] = cblas_ddot(m, Aj_x, 1, x, 1); //c_ij = x* A^j *x
	}

	//Remplir Bm et Bm-1
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			B.B2[i*m + j] = c[i+j];
			B.B1[i*m + j] = c[i+j+1];
		}
	}

	//printf("c à la fin\n"); print_matrice(c, 1, 2*m);
	printf("Bm-1\n"); print_matrice(B.B2, m, m);
	printf("Bm\n"); print_matrice(B.B1, m, m);
	printf("Vm\n"); print_matrice(Vm, m, m);

	free(c);
	free(Aj_x);
	free(B.B1);
	free(B.B2);
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
	double normx = cblas_ddot(n, x, 1, x, 1);
	normalize(x, m);

	printf("x normé\n"); print_matrice(x, 1, m);

	projection(A, n, m, x);

	free(A);
	free(x);
	free(Vm);

	return 0;
}


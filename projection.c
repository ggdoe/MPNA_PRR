#include "tools.h"

//Commande : gcc projection.c tools.c -lm -lcblas -o projection_test.out && ./projection_test.out


double* Vm = NULL;

// A : taille n*n
// B : taille m*m
struct B_proj projection(double *A, int n, int m, const double* x)
{
	struct B_proj B;
	B.B1 = malloc(m*m*sizeof(double));
	B.B2 = malloc(m*m*sizeof(double));

	if(!Vm)
		Vm = malloc(n*m*sizeof(double));

	double* c = malloc(2*m*sizeof(double));
	double* Aj_x = malloc(n*sizeof(double));
	double* tmp = malloc(n*sizeof(double));

	//Remplir c et V_m
	memcpy(Vm, x, n*sizeof(double*));
	memcpy(Aj_x, x, n*sizeof(double*));
	c[0] = 1.;
	for(int j = 1; j < 2*m; j++)
	{
		cblas_dgemv(CblasRowMajor, CblasNoTrans, 
			n, n, 1, A, n, Aj_x, 1, 0, tmp, 1); //Aj_x = A^j *x
		memcpy(Aj_x, tmp, n*sizeof(double*));

		if(j < m) //V_m est de taille m
			memcpy(Vm+j*n, Aj_x, n*sizeof(double));

		c[j] = cblas_ddot(n, Aj_x, 1, x, 1); //c_ij = x* A^j *x
	}

	//Remplir Bm et Bm-1
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			B.B1[i*m + j] = c[i+j];
			B.B2[i*m + j] = c[i+j+1];
		}
	}

	free(c);
	free(Aj_x);
	free(tmp);
	return B;
}


#include "tools.h"

//Commande : gcc projection.c tools.c -lm -lcblas -o projection_test.out && ./projection_test.out


// double* Vm = NULL;

// A : taille n*n
// B : taille m*m
// en sortie x est modifié
void projection(struct projection *p, double *restrict A, int n, int m, double *restrict  x)
{
	double* c = malloc(2*m*sizeof(double));

	for(int j = 0; j < m; j++)
	{
		memcpy(p->Vm + j*n, x, n*sizeof(double));

		cblas_dgemv(CblasRowMajor, CblasNoTrans, 
			n, n, 1, A, n, p->Vm + j*n /*=x*/ , 1, 0, x, 1); //Aj_x = A^j *x

		c[2 * j] = cblas_ddot(n, p->Vm + j*n, 1, p->Vm + j*n, 1); //c_ij = x* A^j *x
		c[2 * j + 1] = cblas_ddot(n, x, 1, p->Vm + j*n, 1); //c_ij = x* A^j *x

	}
	
	// version moins performante, mais plus precise ?
	// double* tmp = malloc(n*sizeof(double));
	// c[0] = 1.;
	// for(int j = 1; j < 2*m; j++)
	// {
	// 	cblas_dgemv(CblasRowMajor, CblasNoTrans, 
	// 		n, n, 1, A, n, Aj_x, 1, 0, tmp, 1); //Aj_x = A^j *x
	// 	memcpy(Aj_x, tmp, n*sizeof(double*));

	// 	if(j < m) //V_m est de taille m
	// 		memcpy(Vm+j*n, Aj_x, n*sizeof(double));

	// 	c[j] = cblas_ddot(n, Aj_x, 1, x, 1); //c_ij = x* A^j *x
	// }

	//Remplir Bm et Bm-1
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			p->B1[i*m + j] = c[i+j];
			p->B2[i*m + j] = c[i+j+1];
		}
	}

	free(c);
}


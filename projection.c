#include "tools.h"

extern double *_tmp_lwork; // taille > 2*m

// en sortie x est modifié
void projection(struct projection *p, double *restrict A, int n, int m, double *restrict  x)
{
	double* c = _tmp_lwork;

	for(int j = 0; j < m; j++)
	{
		memcpy(p->Vm + j*n, x, n*sizeof(double));

		cblas_dgemv(CblasRowMajor, CblasNoTrans, 
			n, n, 1, A, n, p->Vm + j*n /*=x*/ , 1, 0, x, 1); //Aj_x = A^j *x

		c[2 * j] = cblas_ddot(n, p->Vm + j*n, 1, p->Vm + j*n, 1); //c_ij = x* A^j *x
		c[2 * j + 1] = cblas_ddot(n, x, 1, p->Vm + j*n, 1); //c_ij = x* A^j *x

	}

	//Remplir Bm et Bm-1
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			p->B1[i*m + j] = c[i+j];
			p->B2[i*m + j] = c[i+j+1];
		}
	}
}


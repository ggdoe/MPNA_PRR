#include "tools.h"

//Commande : gcc calcul_residu.c tools.c -lm -lcblas -o calcul_residu.out && ./calcul_residu.out

// A : taille m*m
// q : taille m*m
// lambda : taille 1*m
// Retourne le max des résidus au carré
double* calcul_residu(int n, int m, const double* A, const double* q, const double* lambda)
{
	double *residu = malloc(m * sizeof(double));
	double* lambda_q = malloc(n*sizeof(double));

	for (int i = 0; i < m; i++)
	{
		memcpy(lambda_q, q+i*n, n*sizeof(double)); //copier q_i dans lambda_q
		
		//A_q = A * q_i - lambda_i * q_i 
		cblas_dgemv(CblasRowMajor, CblasNoTrans, n, n, 1, A, n, (q + i*n), 1, -lambda[i], lambda_q, 1); 
		
		residu[i] = cblas_ddot(m, lambda_q, 1, lambda_q, 1);
	}

	free(lambda_q);

	return residu;
}

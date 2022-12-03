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

// choisie le nouveau x : combinaison linéaire en fonction des residus calculé
void get_new_x(int n, int m, double *x, double *residu, double *q)
{

		// plus un residu est faible, plus on favorise son vecteurs de ritz associé
	for (int i = 0; i < m; i++)
		residu[i] = 1 / residu[i];

		// normalisation avec norme-1 ou norme-2
	// cblas_dscal(m, 1 / cblas_dasum(m, residu, 1), residu, 1);
	// normalize(residu,m);


		// 3 méthode de choix : max, pondéré, random
	memcpy(x, q + n * cblas_idamax(m, residu, 1), n * sizeof(double));
	// memset(x,0,n*sizeof(double));
	// for (int i = 0; i < m; i++)
	// 	cblas_daxpy(n, residu[i], q + i * n, 1, x, 1);
	// 	cblas_daxpy(n, drand48(), q + i * n, 1, x, 1);

	// étrange : l'erreur augmente avec les restarts, puis elle chute fortement (750900 --> 4.08909e-13)


	normalize(x,n);
}

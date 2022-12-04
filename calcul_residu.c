#include "tools.h"

extern double *_tmp_lwork; // size > n*m
extern void *_tmp_m; 

// A : taille m*m
// q : taille m*m
// lambda : taille 1*m
// Retourne le max des résidus au carré
double* calcul_residu(int n, int m, const double *restrict A, struct spectre *restrict spectre)
{
	const double* q = spectre->vec_p;
	const double* lambda = spectre->vp;

	double *residu = (double*) _tmp_m;
	double* lambda_q = _tmp_lwork;

	// #pragma omp parallel for
	for (int i = 0; i < m; i++)
	{
		memcpy(lambda_q + i*n, q+i*n, n*sizeof(double)); //copier q_i dans lambda_q
		
		//A_q = A * q_i - lambda_i * q_i 
		// on décale lambda_q pour éviter les conflits des threads (et on a la place)
		DGEMV(CblasRowMajor, CblasNoTrans, n, n, 1, A, n, (q + i*n), 1, -lambda[i], lambda_q + i*n, 1); 
		
		residu[i] = DDOT(m, lambda_q + i*n, 1, lambda_q + i*n, 1);
	}
	return residu;
}

// choix du nouveau vecteur x : combinaison linéaire en fonction des résidus calculés
void get_new_x(int n, int m, double *restrict x, double *restrict residu, double *restrict vec_p)
{
		// plus un résidu est faible, plus on favorise son vecteur de ritz associé
	// #pragma omp parallel for
	for (int i = 0; i < m; i++)
		residu[i] = 1 / residu[i];

		// normalisation avec norme-1 ou norme-2
	// DSCAL(m, 1 / DASUM(m, residu, 1), residu, 1);
	// normalize(residu,m);


		// 3 méthodes de choix : max, pondéré, random
	// memcpy(x, vec_p + n * cblas_idamax(m, residu, 1), n * sizeof(double));
	memset(x,0,n*sizeof(double));
	for (int i = 0; i < m; i++) // non parallélisable, daxpy n'est pas atomic
		// DAXPY(n, residu[i], vec_p + i * n, 1, x, 1);
		DAXPY(n, drand48(), vec_p + i * n, 1, x, 1);

	normalize(x,n);
}


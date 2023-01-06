#include "tools.h"

extern double *_tmp_lwork; // taille > n*m
extern double *_tmp_mm;
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
		// residu[i] = DDOT(m, lambda_q + i*n, 1, lambda_q + i*n, 1) / lambda[i];
	}
	return residu;
}

// static int cmp_residu(const void *a, const void* b){
// 	double *residu = (double*) _tmp_m;
// 	return residu[*(int*)a] > residu[*(int*)b];
// }

// choix du nouveau vecteur x : combinaison linéaire en fonction des résidus calculés
void get_new_x(int n, int m, double *restrict x, double *restrict residu, double *restrict vec_p)
{

	// plusieurs méthodes de choix : max, pondéré, random
	
	///// rand_vector(n, x);
	for(int i = 0; i < n; i++)
		x[i] = 2 * drand48() - 1.;

	///------
	// memcpy(x, vec_p + n * cblas_idamax(m, residu, 1), n * sizeof(double));

	///------
	// int *ordre_vp = (int*)_tmp_mm;
	// for (int i = 0; i < m; i++) // index initiaux
	// 	ordre_vp[i] = i;

	// qsort(ordre_vp, m, sizeof(int), cmp_residu); // on trie les index en fonction des residu

	// print_matrice(residu, 1, m);
	// for (int i = 0; i < m; i++)
	// 	printf("%d\t", ordre_vp[i]);

	// for (int i = 0; i < m / 2; i++) // on ne garde que vecteur les plus précis
	// 		DAXPY(n, drand48() + 0.1, vec_p + ordre_vp[i] * n, 1, x, 1);

	///------
	// double *inv_residu = _tmp_mm;
	
		// plus un résidu est faible, plus on favorise son vecteur de ritz associé
	// #pragma omp parallel for
	// for (int i = 0; i < m; i++)
	// 	inv_residu[i] = 1 / residu[i];

		// normalisation avec norme-1 ou norme-2
	// normalize(inv_residu,m);
	// DSCAL(m, 1 / DASUM(m, inv_residu, 1), inv_residu, 1);

	// for (int i = 0; i < m; i++) // non parallélisable, daxpy n'est pas atomic
	// 	// if(residu[i] < DASUM(m, residu, 1) / m)
	// 	if(inv_residu[i] > DASUM(m, inv_residu, 1) / m)
			// DAXPY(n, inv_residu[i], vec_p + i * n, 1, x, 1);
			// DAXPY(n, 2*drand48() - 1., vec_p + i * n, 1, x, 1);

	// normalize(x,n);
}


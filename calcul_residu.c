#include "calcul_residu.h"

//Commande : gcc calcul_residu.c tools.c -lm -lcblas -o calcul_residu.out && ./calcul_residu.out

// A : taille m*m
// q : taille m*m
// lambda : taille 1*m
// Retourne le max des résidus au carré
double calcul_max_residu(int m, const double* A, const double* q, const double* lambda)
{
	double max = 0.;
	double* lambda_q = malloc(m*sizeof(double));

	for (int i = 0; i < m; i++)
	{
		memcpy(lambda_q, q+i*m, m*sizeof(double)); //copier q_i dans lambda_q
		
		//A_q = A * q_i - lambda_i * q_i 
		cblas_dgemv(CblasRowMajor, CblasNoTrans, m, m, 1, A, m, q+i*m, 1, -lambda[i], lambda_q, 1); 
		
		double max_tmp = cblas_ddot(m, lambda_q, 1, lambda_q, 1);
		if (max_tmp > max)
			max = max_tmp;
	}

	free(lambda_q);

	return max;
}

// main de test
/*
int main(int argc, char* argv[])
{
	int m=3, n=3;
	double max_residu;
	double* A = malloc(m*m*sizeof(double));
	double* q = malloc(m*m*sizeof(double));
	double* lambda = malloc(m*sizeof(double));
	for (size_t i = 0; i < m; i++)
	{
		lambda[i] = (i+1)*(i+1)*(i+1);
		for (size_t j = 0; j < m; j++)
		{
			A[i*m+j] = i+j+1;
			q[i*m+j] = i*i-2*j*i+j+1;
		}
		normalize(q + i * m, m);
	}

	printf("A\n"); print_matrice(A, m, m);
	printf("q\n"); print_matrice(q, m, m);
	printf("lambda\n"); print_matrice(lambda, 1, m);

	max_residu = calcul_max_residu(m, A, q, lambda);

	printf("residu\n%lf\n", max_residu);

	free(A); free(q); free(lambda);

	return EXIT_SUCCESS;
}
*/

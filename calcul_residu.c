#include "calcul_residu.h"

//Commande : gcc calcul_residu.c tools.c -lm -lcblas -o calcul_residu.out && ./calcul_residu.out

// A : taille m*m
// q : taille m*m
// lambda : taille 1*m
// Retourne le max des résidus
double calcul_max_residu(int m, const double* A, const double* q, const double* lambda)
{
	double max = 0.0, max_tmp = 0.0;
	double* A_q = malloc(m*sizeof(double));
	double* lambda_q = malloc(m*sizeof(double));

	for (int i = 0; i < m; i++)
	{
		memset(A_q, 0.0, m*sizeof(double));
		memcpy(lambda_q, q+i*m, m*sizeof(double)); //copier q_i dans lambda_q

		cblas_dgemv(CblasRowMajor, CblasNoTrans, m, m, 1, A, m, q+i*m, 1, 1, A_q, 1); //A_q = A * q_i
		cblas_dscal(m, lambda[i], lambda_q, 1); //lambda_q = lambda_i * q_i
		cblas_daxpy(m, -1, lambda_q, 1, A_q, 1); //A_q = A_q - lambda_q
		max_tmp = sqrt(cblas_ddot(m, A_q, 1, A_q, 1));

		if (max_tmp > max)
		{
			max = max_tmp;
		}
	}

	free(A_q);
	free(lambda_q);

	return max;
}

// main de test
/*
int main(int argc, char* argv[])
{
	int m=3, n=3;
	double epsilon = 1e-9, max_residu;
	double* A = malloc(m*m*sizeof(double));
	double* q = malloc(m*m*sizeof(double));
	double* lambda = malloc(m*sizeof(double));

	for (size_t i = 0; i < m; i++)
	{
		lambda[i] = (i+1)*(i+1)*(i+1);
		for (size_t j = 0; j < m; j++)
		{
			A[i*m+j] = i+j+1;
			q[i*m+j] = 2*(i+j+1);
		}
	}

	printf("A\n"); print_matrice(A, m, m);
	printf("q\n"); print_matrice(q, m, m);
	printf("lambda\n"); print_matrice(lambda, 1, m);

	max_residu = calcul_max_residu(m, A, q, lambda);

	free(A); free(q); free(lambda);

	return EXIT_SUCCESS;
}
*/

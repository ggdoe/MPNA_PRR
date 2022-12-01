#include <cblas.h>
#include <lapacke.h>
#include <string.h>
#include "tools.h"

// extern
struct B_proj {
	double *B1;
	double *B2;
};

void resolve_sev(struct B_proj *B, int m)
{
	// var pour LAPACK
	int info;
	int LWORK = m*m;
	double *F = malloc(m*m * sizeof(double));
	double *WORK = malloc(LWORK * sizeof(double));
	int *ipiv = malloc(m * sizeof(int));

	// printf("----11\n");
	// print_matrice(B->Bm1, m,m);
	// printf("----\n");

	// i) inversion matrice Bm1
	LAPACK_dgetrf(&m, &m, B->B2, &m, ipiv, &info);
	LAPACK_dgetri(&m, B->B2, &m, ipiv, WORK, &LWORK, &info);

	// print_matrice(B->Bm, m,m);
	// printf("----dot\n");

	// 2) F = inv(Bm1) * Bm
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, m, m, 1, B->B2, m, B->B1, m, 0, F, m);
	memcpy(B->B2, F, m*m*sizeof(double)); // A SUPPR  : déplacement de F dans Bm1 (c'était pour tester le resultat)
	printf("----F\n");
	print_matrice(F, m,m);
	
	// 3) calcule du spectre de F
	 /// F EST PAS FORCEMENT SYMETRIC
		// --> dgeev


	////// TODO ///////
	// double *G = malloc(m*m * sizeof(double));
	double *vp_r = malloc(m * sizeof(double));
	double *vp_i = malloc(m * sizeof(double));
	double *vecp = malloc(m * m * sizeof(double));
	int lwork = 4 * m;
	double *work = malloc(lwork * sizeof(double));
	
	// on veut les vecteur propre à gauche 
	// pour éviter de transposer A car fortran est en colmajor
	LAPACK_dgeev("V", "N", &m, F, &m, vp_r, vp_i, vecp,
				&m, NULL, &m, work, &lwork, &info);

	for(int i = 0; i < m; i++)
		if(vp_i[i] > epsilon){
			printf("valeur propre non reelle.\n");
			break;
		}

	printf("work : %lf / %d -- info : %d \n", work[0], lwork, info);

	printf("----vp\n");
	print_matrice(vp_r, 1,m);
	// print_matrice(vp_i, 1,m);

	printf("----vecp\n");
	print_matrice(vecp, m,m);

	free(WORK);
	free(ipiv);
}

// int main()
// {
// 	double A[] = {1.,2.,2.,1.};
// 	double A2[] = {2.,3.,3.,2.};
// 	double A3[] = {1., 2.,3.,2.,2., 4., 3., 4., 3.};
// 	double A4[] = {2., 2.,3.,2.,3., 4., 3., 4., 5.};

// 	int m = 3;
// 	struct B_proj B;
// 	B.Bm1 = A3;
// 	B.Bm = A4;
// 	// print_matrice(A3, m, m);
// 	resolve_sev(&B, m);
// 	// print_matrice(A, m, m);
	
// 	return 0;

// }

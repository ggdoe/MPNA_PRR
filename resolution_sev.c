#include "tools.h"


struct spectre resolution_sev(struct projection *B, int m)
{
	// var pour LAPACK
	int info;
	int LWORK = m*m;
	double *F = malloc(m*m * sizeof(double));
	double *WORK = malloc(LWORK * sizeof(double));
	int *ipiv = malloc(m * sizeof(int));

	double *vp_r = malloc(m * sizeof(double));
	double *vp_i = malloc(m * sizeof(double));
	double *vecp = malloc(m * m * sizeof(double));
	int lwork = m * (m < 4 ? 4 : m);

	// i) inversion matrice Bm1
	LAPACK_dgetrf(&m, &m, B->B1, &m, ipiv, &info);
	LAPACK_dgetri(&m, B->B1, &m, ipiv, WORK, &LWORK, &info);
	
	// 2) F = inv(Bm1) * Bm
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, m, m, 1, B->B1, m, B->B2, m, 0, F, m);

	// 3) calcule du spectre de F

	// on veut les vecteur propre à gauche pour éviter 
	// de transposer A car fortran est en colmajor
	LAPACK_dgeev("V", "N", &m, F, &m, vp_r, vp_i, vecp,
				&m, NULL, &m, WORK, &lwork, &info);
	// vecp : vecteur propre selon les lignes

	for(int i = 0; i < m; i++)
		if(vp_i[i] > epsilon){
			printf("resolution_sev.c : valeur propre non reelle. Echec de l'algorithme.\n");
			break;
		}

	// free(WORK);
	// free(ipiv);
	struct spectre spectre = {vp_r, vecp};
	return spectre;
}

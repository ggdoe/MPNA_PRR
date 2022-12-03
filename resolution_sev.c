#include "tools.h"

extern int LWORK;
extern double *_tmp_mm;
extern double *_tmp_lwork;
extern void *_tmp_m; // ipiv ou vp_i

void resolution_sev(struct spectre *spectre, struct projection *B, int m)
{
	// var pour LAPACK
	int info;

	// alias variables
	double *const F = _tmp_mm;
	double *const vp_i = (double*)_tmp_m;
	double *const vp_r = spectre->vp;
	double *const vecp = spectre->vec_p;
	int    *const ipiv = (int*)_tmp_m;

	// i) inversion matrice Bm1 
	LAPACK_dgetrf(&m, &m, B->B1, &m, ipiv, &info);
	LAPACK_dgetri(&m, B->B1, &m, ipiv, _tmp_lwork, &LWORK, &info);
	
	// 2) F = inv(Bm1) * Bm
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, m, m, 1, B->B1, m, B->B2, m, 0, F, m);

	// 3) calcule du spectre de F

	// on veut les vecteur propre à gauche pour éviter 
	// de transposer A car fortran est en colmajor
	LAPACK_dgeev("V", "N", &m, F, &m, vp_r, vp_i, vecp,
				&m, NULL, &m, _tmp_lwork, &LWORK, &info);
	// vecp : vecteur propre selon les lignes

	for(int i = 0; i < m; i++)
		if(vp_i[i] > epsilon){
			printf("resolution_sev.c : valeur propre non reelle. Echec de l'algorithme.\n");
			break;
		}
}

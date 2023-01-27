#include "tools.h"

extern int LWORK;
extern double *_tmp_mm;
extern double *_tmp_lwork;
extern void *_tmp_m; // ipiv ou vp_i


void resolution_sev(struct spectre *restrict spectre, struct projection *restrict B, int m)
{
	// variable pour LAPACK
	int info;

	// alias variables
	double *const F = _tmp_mm;
	double *const vp_i = (double*)_tmp_m;
	double *const vp_r = spectre->vp;
	double *const vecp = spectre->vec_p;
	int    *const ipiv = (int*)_tmp_m;

	// 1) inversion de la matrice Bm1
	DGETRF(&m, &m, B->B1, &m, ipiv, &info);
	DGETRI(&m, B->B1, &m, ipiv, _tmp_lwork, &LWORK, &info);
	
	// 2) F = inv(Bm1) * Bm
	DGEMM(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, m, m, 1, B->B1, m, B->B2, m, 0, F, m);

	// 3) calcul du spectre de F

	// on souhaite les vecteurs propres à gauche pour éviter
	// de transposer A puisque fortran est en colmajor
	DGEEV("V", "N", &m, F, &m, vp_r, vp_i, vecp,
				&m, NULL, &m, _tmp_lwork, &LWORK, &info);
	// vecp : vecteur propre selon les lignes

	// for(int i = 0; i < m; i++)
	// 	vp_r[i] = sqrt(vp_r[i]*vp_r[i] + vp_i[i]*vp_i[i]);


	// static int _printed = 0;
	// if(!_printed)
	// for(int i = 0; i < m; i++)
	// 	if(vp_i[i] > 1){ // >1 pour rester large
	// 		printf("resolution_sev.c : valeur propre non reelle.\n");
	// 		_printed = 1;
	// 		break;
	// 	}
}

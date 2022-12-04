#include "tools.h"

extern double *_tmp_lwork;

// Vm : taille m*m
// u : taille 1*m
// q : taille m*m (tableau retourné)
void retour_espace_depart(int n, int m, const double *restrict Vm, struct spectre *restrict spectre)
{
	double *u = spectre->vec_p;
	double* q = _tmp_lwork;
	
	// nécessité d'exécuter la fonction en colmajor pour avoir les vecteurs de ritz
	// selon les lignes de q alors que nous sommes en rowmajor
	DGEMM(CblasColMajor, CblasNoTrans, CblasNoTrans,
		n, m, m, 1, Vm, n, u, m, 0, q, n); //qi = Vm * ui
	// Vm : les vecteurs sont selon les lignes (n element par vecteur)
	// u : les vecteurs sont selon les lignes (m element par vecteur)
	// q : les vecteurs de ritz sont selon les lignes (n element par vecteur)

	// optionnel : normalisation des vecteurs de ritz
	// #pragma omp parallel for
	for(int i = 0; i < m; i++)
		normalize(q + i*n, n);

	double *swap = _tmp_lwork;
	_tmp_lwork = spectre->vec_p;
	spectre->vec_p = swap;
}

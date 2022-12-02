#include "tools.h"

//Commande : gcc retour_espace_depart.c tools.c -lcblas -o retour_espace_depart.out && ./retour_espace_depart.out

// Vm : taille m*m
// u : taille 1*m
// q : taille m*m (tableau retourné)
void* retour_espace_depart(int n, int m, const double* Vm, const double* u)
{
	double* q = malloc(n*m*sizeof(double));
	
	// cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
	// 	n, m, m, 1, Vm, m, u, m, 0, q, m); //qi = Vm * ui

	// magie noire pour avoir les vecteurs résultat en row major
	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans,
		n, m, m, 1, Vm, n, u, m, 0, q, n); //qi = Vm * ui
	// Vm : les vecteurs sont selon les lignes (n element par vecteur)
	// u : les vecteurs sont selon les lignes (m element par vecteur)
	// q : les vecteurs de ritz sont selon les lignes (n element par vecteur)

	// optionnel : normalisation des vecteurs de ritz
	for(int i = 0; i < m; i++)
		normalize(q + i*n, n);

	return q;
}

#include "tools.h"

//Commande : gcc retour_espace_depart.c tools.c -lcblas -o retour_espace_depart.out && ./retour_espace_depart.out

// Vm : taille m*m
// u : taille 1*m
// q : taille m*m (tableau retourné)
void* ret_esp_dep(int n, int m, const double* Vm, const double* u)
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

	return q;
}

// main de test

// int main(int argc, char* argv[])
// {
// 	int m=3, n=4;
// 	double* Vm = malloc(n*m*sizeof(double));
// 	double* u = malloc(n*m*sizeof(double));
// 	double* q;

// 	for (size_t i = 0; i < n; i++)
// 	{
// 		for (size_t j = 0; j < m; j++)
// 		{
// 			Vm[i*m+j] = j*i+j+i;
// 			u[i*m+j] = 2*i+j+1;
// 		}
// 	}

// 	printf("Vm\n"); print_matrice(Vm, m, n);
// 	printf("u\n"); print_matrice(u, m, m);

// 	q = ret_esp_dep(n, m, Vm, u);
// 	printf("q\n"); print_matrice(q, m, n);

// 	free(Vm); free(u); free(q);

// 	return EXIT_SUCCESS;
// }


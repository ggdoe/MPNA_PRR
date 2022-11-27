#include "retour_espace_depart.h"

//Commande : gcc retour_espace_depart.c tools.c -lcblas -o retour_espace_depart.out && ./retour_espace_depart.out

// Vm : taille m*m
// u : taille 1*m
// q : taille m*m (tableau retourné)
void* ret_esp_dep(int m, const double* Vm, const double* u)
{
	double* q = malloc(m*m*sizeof(double));

	for (int i = 0; i < m; i++)
	{
		cblas_dgemv(CblasRowMajor, CblasNoTrans, m, m, 1, Vm, m, u+i*m, 1, 1, q+i*m, 1); //qi = Vm * ui
	}

	return q;
}

// main de test
/*
int main(int argc, char* argv[])
{
	int m=3, n=3;
	double* Vm = malloc(m*m*sizeof(double));
	double* u = malloc(m*m*sizeof(double));
	double* q = malloc(m*m*sizeof(double));

	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			Vm[i*m+j] = i+j+1;
			u[i*m+j] = 2*(i+j+1);
		}
	}

	printf("Vm\n"); print_matrice(Vm, n, m);
	printf("u\n"); print_matrice(u, n, m);

	q = ret_esp_dep(m, Vm, u);
	printf("q\n"); print_matrice(q, n, m);

	free(Vm); free(u); free(q);

	return EXIT_SUCCESS;
}
*/

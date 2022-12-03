#include "tools.h"
#include <time.h>

#define print_separator(a) printf("--------- %s --------\n", a)

extern double* Vm;

int main()
{
	int n;
	int m = 2;

	double *A = read_matrice("mat2.txt", &n, &n);
	double x[] = {1., 3., 7.};
	// double x[] = {0.606, 0.515, 0.606};
	// double x[] = {-0.610, -0.508, -0.609};
	// double x[] = {0.681,   0.058,   -0.730};
	
	srand48(0);
	int count = 0;

	print_separator("x");
	normalize(x,n);
	print_matrice(x, 1, n);
	print_separator("A");

	begin:
	count++;
	// print_matrice(A, n, n);

	struct B_proj B = projection(A,n,m,x);
	// print_separator("B1");
	// print_matrice(B.B1, m, m);
	// print_separator("B2");
	// print_matrice(B.B2, m, m);

	struct spectre spectre = resolve_sev(&B,m);
	// print_separator("vp");
	// print_matrice(spectre.vp, 1, m);
	// print_separator("vecp");
	// print_matrice(spectre.vec_p, m, m);

	double *q = retour_espace_depart(n,m,Vm,spectre.vec_p);

	// print_separator("vecteur ritz");
	// print_matrice(q, m, n);

	double *residu = calcul_residu(n, m, A, q, spectre.vp);
	// printf("------- residu -------\n");	
	// print_matrice(residu, 1, m);
	double maxres = max(residu,m);
	printf("max : %lg\n", max(residu,m));

	////////////
	// on choisie le nouveau x : combinaison linéaire en fonction des residus

	// plus un residu est faible, plus on favorise son vecteurs de ritz associé
	for (int i = 0; i < m; i++)
		residu[i] = 1 / residu[i];

	// normalisation avec norme-1 ou norme-2
	// cblas_dscal(m, 1 / cblas_dasum(m, residu, 1), residu, 1);
	normalize(residu,m);

	memset(x,0,n*sizeof(double));

	// 3 méthode de choix : max, pondéré, random
	memcpy(x, q + n * cblas_idamax(m, residu, 1), n * sizeof(double));
	// for (int i = 0; i < m; i++)
		// cblas_daxpy(n, residu[i], q + i * n, 1, x, 1);
		// cblas_daxpy(n, drand48(), q + i * n, 1, x, 1);

	// étrange : l'erreur augmente avec les restarts, puis elle chute fortement (750900 --> 4.08909e-13)

	//////////////

	normalize(x,n);
	// print_separator("new x");
	// print_matrice(x, 1, n);

	// c'est la fêtes aux memory leak !
	if(maxres > 1e-5)
		goto begin;

	printf("count : %d\n", count);

	print_separator("final x");
	print_matrice(x, 1, n);

	print_separator("vp");
	print_matrice(spectre.vp, 1, m);

	print_separator("vecteur ritz");
	print_matrice(q, m, n);
	
	free(A);


	return 0;
}


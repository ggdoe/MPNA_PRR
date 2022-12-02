#include "tools.h"

#define print_sep(a) printf("--------- %s --------\n", a)

extern double* Vm;

int main()
{
	int n;
	int m = 3;

	double *A = read_matrice("mat2.txt", &n, &n);
	double x[] = {1., 3., 7.};
	// double x[] = {0.606, 0.515, 0.606};

	print_sep("x");
	normalize(x,n);
	print_matrice(x, 1, n);
	print_sep("A");
	print_matrice(A, n, n);

	struct B_proj B = projection(A,n,m,x);
	print_sep("B1");
	print_matrice(B.B1, m, m);
	print_sep("B2");
	print_matrice(B.B2, m, m);

	struct spectre spectre = resolve_sev(&B,m);
	print_sep("vp");
	print_matrice(spectre.vp, 1, m);
	print_sep("vecp");
	print_matrice(spectre.vec_p, m, m);

	double *q = retour_espace_depart(n,m,Vm,spectre.vec_p);

	print_sep("vecteur ritz");
	print_matrice(q, m, n);

	double ritz_estimate = calcul_max_residu(n, m, A, q, spectre.vp);
	printf("------- ritz estimate -------\n%lg\n", ritz_estimate);

	free(A);

	return 0;
}


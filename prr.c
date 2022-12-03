#include "tools.h"

struct spectre prr(double n, double m, double *restrict A, double *restrict x)
{
	struct projection p;
	struct spectre spectre;
	double *q;
	double *residu;
	double maxres;
	int count = 0;

	p.B1 = malloc(m*m*sizeof(double));
	p.B2 = malloc(m*m*sizeof(double));
	p.Vm = malloc(n*m*sizeof(double));







	print_separator("x");
	normalize(x,n);
	print_matrice(x, 1, n);
	// print_separator("A");

	// + vérifier que A est symétrique, sinon l'algo ne s'applique pas!
	printf("------- residu -------\n");	

	begin:
	count++;
	// print_matrice(A, n, n);

	projection(&p, A, n, m, x);
	// print_separator("B1");
	// print_matrice(p.B1, m, m);
	// print_separator("B2");
	// print_matrice(p.B2, m, m);

	spectre = resolution_sev(&p,m);
	// print_separator("vp");
	// print_matrice(spectre.vp, 1, m);
	// print_separator("vecp");
	// print_matrice(spectre.vec_p, m, m);

	q = retour_espace_depart(n, m, p.Vm, spectre.vec_p);

	// print_separator("vecteur ritz");
	// print_matrice(q, m, n);

	residu = calcul_residu(n, m, A, q, spectre.vp);
	// printf("------- residu -------\n");	
	// print_matrice(residu, 1, m);
	maxres = max(residu,m);
	printf("max : %lg\n", max(residu,m));

	// nouveau vecteur initial :
	get_new_x(n,m,x,residu,q);

	// print_separator("new x");
	// print_matrice(x, 1, n);

	// c'est la fêtes aux memory leak !
	if(maxres > 1e-4)
		goto begin;

	printf("count : %d\n", count);

	print_separator("final x");
	print_matrice(x, 1, n);

	free(spectre.vec_p);
	spectre.vec_p = q;
	
	return spectre;
}


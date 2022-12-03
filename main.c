#include "tools.h"
#include <time.h>

// extern double* Vm;

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
	if(m > n) 
		exit(1);
	
	struct spectre spectre = prr(n, m, A, x);

	print_separator("vp");
	print_matrice(spectre.vp, 1, m);

	print_separator("vecteur ritz");
	print_matrice(spectre.vec_p, m, n);

	free(A);


	return 0;
}


#include "tools.h"
#include <time.h>

int main()
{
	int n;
	int m = 4;
	struct prr_info prrinfo;

	srand48(time(NULL));

	double *A = read_matrice("../mat3.txt", &n, &n);
	double *x;
	// double x[] = {1., 3., 7.};
	struct spectre spectre;
	// for (size_t i = 0; i < 1000; i++)
	{
		// srand48(0);
		x = rand_initial_vector(n);
		spectre = prr(n, m, A, x, &prrinfo, 0, 1e-4);
	}
	

	print_separator("vp");
	print_matrice(spectre.vp, 1, m);

	print_separator("vecteur ritz");
	print_matrice(spectre.vec_p, m, n);

	printf("count : %d\n", prrinfo.nb_it);
	printf("max residu : %lg\n", prrinfo.max_residu);

	free(A);
	free(spectre.vec_p);
	free(spectre.vp);
	free(x);

	return 0;
}


#include "tools.h"

int main(int argc, char **argv)
{
	int n;
	int m = 6;
	struct prr_info prrinfo;

	// init MPI
	int nb_mpi, rank_mpi;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nb_mpi);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank_mpi);

	srand48(rank_mpi);
	// srand48(rank_mpi + time(NULL));
	
	double *A = read_matrice("../mat4.txt", &n, &n);
	double *x;
	// double x[] = {1., 3., 7.};
	struct spectre spectre;
	// for (size_t i = 0; i < 1000; i++)
	{
		// srand48(0);
		x = rand_initial_vector(n);
		spectre = multi_prr(n, m, A, x, &prrinfo, 0, 1e-2, 1000);

		// spectre = prr(n, m, A, x, &prrinfo, 0, 1e-2);
	}

	if(prrinfo.got_result){
		print_separator("vp");
		print_matrice(spectre.vp, 1, m);

		print_separator("vecteur ritz");
		print_matrice(spectre.vec_p, m, n);

		printf("count : %d\n", prrinfo.nb_it);
		printf("max residu : %lg\n", prrinfo.max_residu);
	}

	free(A);
	free(spectre.vec_p);
	free(spectre.vp);
	free(x);

	MPI_Finalize();
	return 0;
}


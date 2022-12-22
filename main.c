#include "tools.h"

int main(int argc, char **argv)
{
	srand48(time(NULL));
	int n, m = 6;
	double* A = NULL, *x = NULL;
	struct prr_info prrinfo;
	struct spectre spectre;

	#ifdef MULTIPRR
		if(argc != 3)
		{
			printf("Utilisation : ./prr_mpi <matrice> <nb_threads>\n");
			printf("<matrice> : fichier à lire contenant la matrice à étudier\n");
			printf("<nb_threads> : nombre de threads à avoir lors d'une exécution en parallèle.\n");
			exit(EXIT_FAILURE);
		}

		char* p_argv = NULL;
		int nb_threads = (int) strtol(argv[2], &p_argv, 10);

		if(*p_argv != '\0' || nb_threads <= 0)
		{
			printf("Erreur : nombre de threads non reconnu. Veuillez indiquer une valeur entière supérieure à 1.\n");
			exit(EXIT_FAILURE);
		}

		// init MPI
		int nb_mpi, rank_mpi;
		MPI_Init(&argc, &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &nb_mpi);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank_mpi);

		A = read_matrice(argv[1], &n, &n);
		omp_set_num_threads(nb_threads);

		//for (size_t i = 0; i < 1000; i++)
		//{
			x = rand_initial_vector(n);
			spectre = multi_prr(n, m, A, x, &prrinfo, 0, 1e-2, 1000);
		//}

		MPI_Finalize();

	#else
		if(argc != 2)
		{
			printf("Utilisation : ./prr <matrice>\n");
			printf("<matrice> : fichier à lire contenant la matrice à étudier\n");
			exit(EXIT_FAILURE);
		}

		A = read_matrice(argv[1], &n, &n);
		omp_set_num_threads(1);

		//for (size_t i = 0; i < 1000; i++)
		//{
			x = rand_initial_vector(n);
			spectre = prr(n, m, A, x, &prrinfo, 0, 1e-2);
		//}

	#endif

	if(prrinfo.got_result){
		print_separator("vp");
		print_matrice(spectre.vp, 1, m);

		print_separator("vecteur ritz");
		print_matrice(spectre.vec_p, m, n);

		printf("count : %d\n", prrinfo.nb_it);
		printf("tps exec : %lf\n", prrinfo.tps_exec);
		printf("max residu : %lg\n", prrinfo.max_residu);
	}

	free(A);
	free(spectre.vec_p);
	free(spectre.vp);
	free(x);

	return 0;
}


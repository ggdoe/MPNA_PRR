#include "tools.h"

int main(int argc, char **argv)
{
	if(argc != 4)
	{
		printf("Utilisation : ./prog <fichier> <seq|par> <nb_threads>\n");
		printf("prog : nom de l'exécutable, ici \"prr\"\n");
		printf("<fichier> : fichier à lire contenant la matrice à étudier\n");
		printf("<seq|par> : exécution sequentielle ou parallèle\n");
		printf("<nb_threads> : nombre de threads à avoir lors d'une exécution en parallèle. Inutile lors d'une exécution en séquentielle et sera remplacé par 1.\n");
		exit(EXIT_FAILURE);
	}

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
	
	double *A = read_matrice(argv[1], &n, &n);
	double *x = NULL;
	// double x[] = {1., 3., 7.};
	struct spectre spectre;

	if(!strcmp(argv[2], "par"))
	{
		omp_set_num_threads(atoi(argv[3]));
		//for (size_t i = 0; i < 1000; i++)
		//{
			x = rand_initial_vector(n);
			spectre = multi_prr(n, m, A, x, &prrinfo, 0, 1e-2, 1000);
		//}
	}

	else if(!strcmp(argv[2], "seq"))
	{
		omp_set_num_threads(1);
		//for (size_t i = 0; i < 1000; i++)
		//{
			x = rand_initial_vector(n);
			spectre = prr(n, m, A, x, &prrinfo, 0, 1e-2);
		//}
	}

	else
	{
		printf("Erreur : argument 1 non reconnu, veuillez choisir entre \"seq\" et \"par\"\n");
		exit(EXIT_FAILURE);
	}

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

	MPI_Finalize();
	return 0;
}


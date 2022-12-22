#include "tools.h"

int main(int argc, char **argv)
{
	srand48(time(NULL));
	int n, m = 6, nb_reps = 1;
	double* A = NULL, *x = NULL;
	double epsilon = 1e-2;
	struct prr_info prrinfo;
	struct spectre spectre;

	#ifdef MULTIPRR
		if(argc != 5)
		{
			printf("Utilisation : ./prr_mpi <matrice> <epsilon> <nb_reps> <freq_msg_mpi>\n");
			printf("<matrice> : fichier à lire contenant la matrice à étudier\n");
			printf("<epsilon> : précision des résidus\n");
			printf("<nb_reps> : nombre de fois que l'algorithme sera exécuté \n");
			printf("<freq_msg_mpi> : entier représentant l'itération où les processus mpi communiquent\n");
			exit(EXIT_FAILURE);
		}
		A = read_matrice(argv[1], &n, &n);
		epsilon = atof(argv[2]);
		nb_reps = atoi(argv[3]);
		int freq_msg_mpi = atoi(argv[4]);

		if(epsilon <= 0)
		{
			printf("Erreur : epsilon <= 0\n");
			exit(EXIT_FAILURE);
		}
		if(nb_reps <= 0)
		{
			printf("Erreur : nb_reps <= 0\n");
			exit(EXIT_FAILURE);
		}
		if(freq_msg_mpi <= 0)
		{
			printf("Erreur : freq_msg_mpi <= 0\n");
			exit(EXIT_FAILURE);
		}

		int nb_mpi, rank_mpi;
		MPI_Init(&argc, &argv);
		MPI_Comm_size(MPI_COMM_WORLD, &nb_mpi);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank_mpi);

		for (size_t i = 0; i < nb_reps; i++)
		{
			x = rand_initial_vector(n);
			spectre = multi_prr(n, m, A, x, &prrinfo, 0, epsilon, freq_msg_mpi);
		}

		MPI_Finalize();

	#else
		if(argc != 4)
		{
			printf("Utilisation : ./prr <matrice> <epsilon> <nb_reps>\n");
			printf("<matrice> : fichier à lire contenant la matrice à étudier\n");
			printf("<epsilon> : précision des résidus\n");
			printf("<nb_reps> : nombre de fois que l'algorithme sera exécuté \n");
			exit(EXIT_FAILURE);
		}
		A = read_matrice(argv[1], &n, &n);
		epsilon = atof(argv[2]);
		nb_reps = atoi(argv[3]);

		if(epsilon <= 0)
		{
			printf("Erreur : epsilon <= 0\n");
			exit(EXIT_FAILURE);
		}
		if(nb_reps <= 0)
		{
			printf("Erreur : nb_reps <= 0\n");
			exit(EXIT_FAILURE);
		}

		for (size_t i = 0; i < nb_reps; i++)
		{
			x = rand_initial_vector(n);
			spectre = prr(n, m, A, x, &prrinfo, 0, epsilon);
		}

	#endif

	if(prrinfo.got_result){
		printf("Résultats de la dernière itération :\n");
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


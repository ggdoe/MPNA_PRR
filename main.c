#include "tools.h"

static long get_nanosec();
static void init_program(int argc, char **argv, double *epsilon, int *freq_msg_mpi, int *nb_reps);

int main(int argc, char **argv)
{
	int n, m, nb_reps;
	int freq_msg_mpi;
	int max_it;
	double *A = NULL, *x = NULL;
	double epsilon = 1e-2;
	struct prr_info prrinfo;
	struct spectre spectre;

	m = 5;
	nb_reps = 1;
	epsilon = 1e-2;
	freq_msg_mpi = 100;
	max_it = 0;

	// init_program(argc, argv, &epsilon, &freq_msg_mpi, &nb_reps);

	A = read_matrice("mat20x20.txt", &n, &n);
	// A = read_matrice(argv[1], &n, &n); // vérification si argv[1] est un fichier existant

	#ifdef MULTIPRR
		int rank_mpi;
		MPI_Init(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank_mpi);

		long seed = get_nanosec() - rank_mpi * (get_nanosec() % 51594);
		srand48(seed);
		// printf("seed : %ld\n", seed);
	#else
		srand48(get_nanosec());
	#endif

	for (size_t i = 0; i < nb_reps; i++)
	{
		x = rand_initial_vector(n);

		#ifdef MULTIPRR
			spectre = multi_prr(n, m, A, x, &prrinfo, max_it, epsilon, freq_msg_mpi);
		#else
			spectre = prr(n, m, A, x, &prrinfo, max_it, epsilon);
		#endif
	}

	if(prrinfo.got_result){
		printf("Résultats de la dernière itération :\n");
		print_separator("vp");
		print_matrice(spectre.vp, 1, m);

		print_separator("vecteur ritz");
		print_matrice(spectre.vec_p, m, n);

	#ifdef MULTIPRR
		printf("rank : %d\n", rank_mpi);
	#endif
		printf("count : %d\n", prrinfo.nb_it);
		printf("tps exec : %lf µs\n", prrinfo.tps_exec);
		printf("max residu : %lg\n", prrinfo.max_residu);

		write_files(prrinfo, n, m, epsilon, max_it, freq_msg_mpi);
	}

	FREE(A);
	FREE(spectre.vec_p);
	FREE(spectre.vp);
	FREE(x);

	#ifdef MULTIPRR
		MPI_Finalize();
	#endif

	return 0;
}

static long get_nanosec()
{
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC_RAW, &time);
	return time.tv_nsec;
}

static void init_program(int argc, char **argv, double *epsilon, int *freq_msg_mpi, int *nb_reps)
{
	*epsilon = atof(argv[2]);
	*nb_reps = atoi(argv[3]);
	*freq_msg_mpi = atoi(argv[4]);
	
	if(argc != 5){
		printf("Utilisation : ./prr_mpi <matrice> <epsilon> <nb_reps> <freq_msg_mpi>\n"\
				"<matrice> : fichier à lire contenant la matrice à étudier\n"\
				"<epsilon> : précision des résidus\n"\
				"<nb_reps> : nombre de fois que l'algorithme sera exécuté \n"\
				"<freq_msg_mpi> : entier représentant l'itération où les processus mpi communiquent\n");
		exit(EXIT_FAILURE);
	}
	if(*epsilon <= 0 || *freq_msg_mpi <= 0 || *nb_reps <= 0){
		printf("Erreur : paramètre(s) négatif(s)\n");
		exit(EXIT_FAILURE);
	}
}

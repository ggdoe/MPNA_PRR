#include "tools.h"

static long get_nanosec();
static struct prgm_config init_program(int argc, char **argv);

int main(int argc, char **argv)
{
	int n;
	double *A = NULL, *x = NULL;

	struct prr_info prrinfo;
	struct spectre spectre;
	struct prgm_config config = init_program(argc, argv);

	A = read_matrice("mat20x20.txt", &n, &n);
	// A = read_matrice(argv[1], &n, &n); // vérification si argv[1] est un fichier existant
	x = MALLOC(n * sizeof(double));

	#ifdef MULTIPRR
		int rank_mpi;
		MPI_Init(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank_mpi);

		long seed = get_nanosec() - rank_mpi * (get_nanosec() % 51594);
		srand48(seed);
		// printf("seed : %ld\n", seed);
	#else
		srand48(get_nanosec());
		config.freq = -1;
	#endif

	for (size_t i = 0; i < config.nb_rep; i++)
	{
		rand_vector(n, x);

		#ifdef MULTIPRR
			spectre = multi_prr(n, A, x, &prrinfo, &config);
		#else
			spectre = prr(n, A, x, &prrinfo, &config);
		#endif
	}

	if(prrinfo.got_result){
		printf("Résultats de la dernière itération :\n");
		print_separator("vp");
		print_matrice(spectre.vp, 1, config.m);

		print_separator("vecteur ritz");
		print_matrice(spectre.vec_p, config.m, n);

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

static struct prgm_config init_program(int argc, char **argv)
{
	struct prgm_config config = {.epsilon = 1e-2,
								 .freq = 100,
								 .m = 5,
								 .max_it = 0,
								 .nb_rep = 1,
								 .filename = NULL};

	for(int i = 1; i < argc - 1; i++)
	{
		if(!strcmp(argv[i], "--epsilon"))
			config.epsilon = atof(argv[++i]);

		else if(!strcmp(argv[i], "--file"))
			config.filename = argv[++i];

		else if(!strcmp(argv[i], "--nb_rep"))
			config.nb_rep = atoi(argv[++i]);

		else if(!strcmp(argv[i], "--freq"))
			config.freq = atoi(argv[++i]);
			
		else if(!strcmp(argv[i], "--m"))
			config.m = atoi(argv[++i]);

		else if(!strcmp(argv[i], "--max_it"))
			config.max_it = atoi(argv[++i]);

		else
			fprintf(stderr, "mauvais paramètre.\n");
	}

	if(argc != 5 || config.filename == NULL){
		printf("Utilisation : ./prr_mpi <matrice> <epsilon> <nb_reps> <freq_msg_mpi>\n"\
				"<matrice> : fichier à lire contenant la matrice à étudier\n"\
				"<epsilon> : précision des résidus\n"\
				"<nb_reps> : nombre de fois que l'algorithme sera exécuté \n"\
				"<freq_msg_mpi> : entier représentant l'itération où les processus mpi communiquent\n");
		exit(EXIT_FAILURE);
	}
	// if(*epsilon <= 0 || *freq_msg_mpi <= 0 || *nb_reps <= 0){
	// 	printf("Erreur : paramètre(s) négatif(s)\n");
	// 	exit(EXIT_FAILURE);
	// }

	return config;
}

#include "tools.h"

static long get_nanosec();
static struct prgm_config init_program(int argc, char **argv);

int main(int argc, char **argv)
{
	int n;
	double *A = NULL, *x = NULL;
	int rank_mpi;


	struct prr_info prrinfo;
	struct spectre spectre;
	struct prgm_config config = init_program(argc, argv);

	// A = read_matrice("mat20x20.txt", &n, &n);
	A = read_matrice(config.filename, &n, &n);

	x = MALLOC(n * sizeof(double));
	#ifdef MULTIPRR
		MPI_Init(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &rank_mpi);

		long seed = get_nanosec() - rank_mpi * (get_nanosec() % 51594);
		srand48(seed);
		// printf("seed : %ld\n", seed);
	#else
		srand48(get_nanosec());
		config.freq = -1;
		rank_mpi = 0;
	#endif

	init_log();
	if(rank_mpi == 0) // seul le master log la config
		log_config(&config, n);

	for (size_t i = 0; i < config.nb_rep; i++)
	{
		rand_vector(n, x);

		#ifdef MULTIPRR
			spectre = multi_prr(n, A, x, &prrinfo, &config);
		#else
			spectre = prr(n, A, x, &prrinfo, &config);
		#endif

		if(prrinfo.got_result)
			log_result(prrinfo);
		
		if(i + 1 < config.nb_rep){
			FREE(spectre.vp);
			FREE(spectre.vec_p);
		}
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
	}

	FREE(A);
	FREE(spectre.vec_p);
	FREE(spectre.vp);
	FREE(x);
	close_result();

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
			fprintf(stderr, "Mauvais paramètre.\n");
	}

	if(config.filename == NULL){
		printf("Utilisation : %s <--options value>\n"\
				"<--m : int> : nombre de vecteurs de ritz à extraire.\n"\
				"<--max_it : int> : nombre d'itération maximum.\n"\
				"<--file : string> : fichier à lire contenant la matrice à étudier\n"\
				"<--epsilon : float> : précision des résidus\n"\
				"<--nb_rep : int> : nombre de fois que l'algorithme sera exécuté \n"\
				"<--freq : int> : fréquence des communications mpi (en nbr d'itération)\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// printf("filename=%s\tepsilon=%lg\tfreq=%d\tm=%d\tmax_it=%d\tnb_rep=%d\n\n", config.filename, config.epsilon, config.freq, config.m, config.max_it, config.nb_rep);



	return config;
}

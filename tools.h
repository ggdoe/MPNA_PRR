#ifndef TOOLS_H
#define TOOLS_H

#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
// #include <omp.h>

#ifdef MKL
	#include <mkl.h>
#else
	#include <cblas.h>
	#include <lapacke.h>
#endif

#ifdef MULTIPRR
	#include <mpi.h>
#endif

#ifdef MKL
	#define MALLOC(x) mkl_malloc(x, 64)
	#define FREE(x) mkl_free(x)	
#else
	#define MALLOC(x) aligned_alloc(64, x)
	#define FREE(x) free(x)	
#endif

#define DGEMV cblas_dgemv
#define DGEMM cblas_dgemm
#define DSCAL cblas_dscal
#define DASUM cblas_dasum
#define DDOT  cblas_ddot
#define DAXPY cblas_daxpy

#ifdef MKL
	#define DGETRF dgetrf
	#define DGETRI dgetri
	#define DGEEV  dgeev
#else
	#define DGETRF LAPACK_dgetrf
	#define DGETRI LAPACK_dgetri
	#define DGEEV LAPACK_dgeev
#endif


#define print_separator(a) printf("--------- %s --------\n", a)

struct prgm_config{
	char *filename;
	int m, nb_rep, freq, max_it;
	double epsilon;
};

struct projection {
	double *B1;
	double *B2;
	double *Vm;
};

struct spectre{
	double *vp;
	double *vec_p;
};

struct prr_info{
	double max_residu;
	int nb_it;
	double tps_exec;
	int got_result; // est ce que ce process MPI possède le résultat
};

struct spectre prr(int n, double *restrict A, double *restrict x, struct prr_info *restrict prrinfo, struct prgm_config *restrict config);
struct spectre multi_prr(int n, double *restrict A, double *restrict x, struct prr_info *restrict prrinfo, struct prgm_config *restrict config);

// etape algorithme 
void projection(struct projection *p, double *restrict A, int n, int m, double*restrict  x);
void resolution_sev(struct spectre *restrict spectre, struct projection *restrict B, int m);
void retour_espace_depart(int n, int m, const double *restrict Vm, struct spectre *restrict spectre);
double* calcul_residu(int n, int m, const double *restrict A, struct spectre *restrict spectre);
void get_new_x(int n, int m, double *restrict x, double *restrict residu, double *restrict vec_p);

// tools
void normalize(double *x, int n);
double max(double *x, int n);
void rand_vector(int n, double *x);
void print_matrice(double *A, int n, int m);
double * read_matrice(char *filename, int *n, int *m);
void write_files(struct prr_info prrinfo, int n, int m, double epsilon, int max_it, int freq_msg_mpi);

#endif

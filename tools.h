#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cblas.h>
#include <lapacke.h>
#include <omp.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

#define DGEMV cblas_dgemv
#define DGEMM cblas_dgemm
#define DSCAL cblas_dscal
#define DASUM cblas_dasum
#define DDOT cblas_ddot
#define DAXPY cblas_daxpy
#define DGETRF LAPACK_dgetrf
#define DGETRI LAPACK_dgetri
#define DGEEV LAPACK_dgeev

#define epsilon 1.e-3
#define print_separator(a) printf("--------- %s --------\n", a)

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

struct spectre prr(int n, int m, double *restrict A, double *restrict x, struct prr_info *restrict prrinfo, int max_it, double _epsilon);
struct spectre multi_prr(int n, int m, double *restrict A, double *restrict x, struct prr_info *restrict prrinfo, int max_it, double _epsilon, int interval_comm);

// etape algorithme 
void projection(struct projection *p, double *restrict A, int n, int m, double*restrict  x);
void resolution_sev(struct spectre *restrict spectre, struct projection *restrict B, int m);
void retour_espace_depart(int n, int m, const double *restrict Vm, struct spectre *restrict spectre);
double* calcul_residu(int n, int m, const double *restrict A, struct spectre *restrict spectre);
void get_new_x(int n, int m, double *restrict x, double *restrict residu, double *restrict vec_p);

// tools
void normalize(double *x, int n);
double max(double *x, int n);
double *rand_initial_vector(int n);
void print_matrice(double *A, int n, int m);
double * read_matrice(char *filename, int *n, int *m);

// void sym_to_std(double *in, double *out, int m);
// void std_to_sym(double *in, double *out, int m);

#endif

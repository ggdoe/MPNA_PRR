#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cblas.h>
#include <lapacke.h>
#include <math.h>

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
};

struct spectre prr(int n, int m, double *restrict A, double *restrict x, struct prr_info *prrinfo, int max_it);

void print_matrice(double *A, int n, int m);
double * read_matrice(char *filename, int *n, int *m);
void projection(struct projection *p, double *restrict A, int n, int m, double*restrict  x);
void normalize(double *x, int n);
void resolution_sev(struct spectre *spectre, struct projection *B, int m);

void retour_espace_depart(int n, int m, const double* Vm, struct spectre* spectre);

void get_new_x(int n, int m, double *x, double *residu, double *q);

double* calcul_residu(int n, int m, const double* A, struct spectre *spectre);
void ret_esp_dep(int n, int m, const double* Vm, const double* u);
double max(double *x, int n);
double *rand_initial_vector(int n);


// void sym_to_std(double *in, double *out, int m);
// void std_to_sym(double *in, double *out, int m);

#endif

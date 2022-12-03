#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cblas.h>
#include <lapacke.h>
#include <math.h>

#define epsilon 1.e-3

struct B_proj {
	double *B1;
	double *B2;
};

struct spectre{
	double *vp;
	double *vec_p;
};


void print_matrice(double *A, int n, int m);
double * read_matrice(char *filename, int *n, int *m);
struct B_proj projection(double *A, int n, int m, const double* x);
void normalize(double *x, int n);
struct spectre resolve_sev(struct B_proj *B, int m);
void* retour_espace_depart(int n, int m, const double* Vm, const double* u);

double* calcul_residu(int n, int m, const double* A, const double* q, const double* lambda);
void* ret_esp_dep(int n, int m, const double* Vm, const double* u);
double max(double *x, int n);

// void sym_to_std(double *in, double *out, int m);
// void std_to_sym(double *in, double *out, int m);

#endif

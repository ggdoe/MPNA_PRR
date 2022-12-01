#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cblas.h>
#include <math.h>

#define epsilon 1.e-3

void print_matrice(double *A, int n, int m);
double * read_matrice(char *filename, int *n, int *m);
void* projection(double *A, int n, int m, const double* x);
void normalize(double *x, int n);

// void sym_to_std(double *in, double *out, int m);
// void std_to_sym(double *in, double *out, int m);

#endif

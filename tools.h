#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cblas.h>
#include <math.h>

void print_matrice(double *A, int n, int m);
double * read_matrice(char *filename, int *n, int *m);

#endif

#include <cblas.h>
#include <lapacke.h>
#include <string.h>
#include "tools.h"

// extern
struct B_proj {
	double *Bm;
	double *Bm1;
};

void resolve_sev(struct B_proj *B, int m)
{
	int info;
	int LWORK = m*m;
	double *F = malloc(m*m * sizeof(double));
	double *WORK = malloc(LWORK * sizeof(double));
	int *ipiv = malloc(m * sizeof(int));


	LAPACK_dgetrf(&m, &m, B->Bm1, &m, ipiv, &info);
	LAPACK_dgetri(&m, B->Bm1, &m, ipiv, WORK, &LWORK, &info);

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, m, m, 1, B->Bm1, m, B->Bm, m, 0, F, m);
	memcpy(B->Bm1, F, m*m*sizeof(double));


	free(WORK);
	free(ipiv);
}

int main()
{
	double A[] = {1.,2.,2.,1.};
	double A2[] = {2.,3.,3.,2.};
	struct B_proj B;
	B.Bm1 = A;
	B.Bm = A2;
	print_matrice(A, 2, 2);
	resolve_sev(&B, 2);
	print_matrice(A, 2, 2);
	
	return 0;

}

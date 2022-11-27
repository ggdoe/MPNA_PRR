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
	// var pour LAPACK
	int info;
	int LWORK = m*m;
	double *F = malloc(m*m * sizeof(double));
	double *WORK = malloc(LWORK * sizeof(double));
	int *ipiv = malloc(m * sizeof(int));

	// i) inversion matrice Bm1
	LAPACK_dgetrf(&m, &m, B->Bm1, &m, ipiv, &info);
	LAPACK_dgetri(&m, B->Bm1, &m, ipiv, WORK, &LWORK, &info);

	// 2) F = inv(Bm1) * Bm
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, m, m, 1, B->Bm1, m, B->Bm, m, 0, F, m);
	memcpy(B->Bm1, F, m*m*sizeof(double)); // A SUPPR  : déplacement de F dans Bm1 (c'était pour tester le resultat)

	// 3) calcule du spectre de F
	////// TODO ///////
		// - copier F dans une autre matrice, stocker au format symmétric : https://www.ibm.com/docs/en/essl/6.2?topic=matrix-symmetric-storage-representation
		// - calcule du spectre avec 'dsyev' : https://netlib.org/lapack/explore-html/d2/d8a/group__double_s_yeigen_ga442c43fca5493590f8f26cf42fed4044.html#ga442c43fca5493590f8f26cf42fed4044
		// - mettre le résultat dans une nouvelle structure (par exemple) et renvoyer le résultat
	////////////

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

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

	printf("----11\n");
	print_matrice(B->Bm1, m,m);
	printf("----\n");

	// i) inversion matrice Bm1
	LAPACK_dgetrf(&m, &m, B->Bm1, &m, ipiv, &info);
	LAPACK_dgetri(&m, B->Bm1, &m, ipiv, WORK, &LWORK, &info);

	print_matrice(B->Bm, m,m);
	printf("----dot\n");

	// 2) F = inv(Bm1) * Bm
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, m, m, 1, B->Bm1, m, B->Bm, m, 0, F, m);
	memcpy(B->Bm1, F, m*m*sizeof(double)); // A SUPPR  : déplacement de F dans Bm1 (c'était pour tester le resultat)
	print_matrice(F, m,m);
	
	// 3) calcule du spectre de F
	 /// F EST PAS FORCEMENT SYMETRIC
		// --> dgeev


	////// TODO ///////
	double *G = malloc(m*m * sizeof(double));
	memset(G,0,m*m*sizeof(double));


	std_to_sym(F,G,m);
	memset(F,0,m*m*sizeof(double));
	sym_to_std(G,F,m);

	// LAPACK_dtpttr("U", &m, F, G, &m, &info);
	// LAPACK_dtrttp("U", &m, G, &m, F, &info);
	// if(info != 0)
	// 	printf("%d \n", info);
	printf("----S\n");
	print_matrice(F, m,m);
	// print_matrice(G, m,m);


	// for (int i = 0; i < m * m; i++)
	// {
	// 	if (i % m == 0)
	// 		printf("\n");

	// 	printf("%lf " ,F[i]);
	// }
	// printf("\n");
	
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
	double A3[] = {1., 2.,3.,2.,2., 4., 3., 4., 3.};
	double A4[] = {2., 2.,3.,2.,3., 4., 3., 4., 5.};

	int m = 3;
	struct B_proj B;
	B.Bm1 = A3;
	B.Bm = A4;
	print_matrice(A3, m, m);
	resolve_sev(&B, m);
	// print_matrice(A, m, m);
	
	return 0;

}

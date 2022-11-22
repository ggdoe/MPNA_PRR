#include <stdio.h>
#include <cblas.h>

int main()
{
	double a[] = {1,2,3,4};
	double b[] = {1,2,3,4};
	double c[] = {0,0,0,0};

	cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, 
		2, 2, 2, 1., a, 2, b, 2, 0, c, 2);

	for(int i = 0; i < 2; i++){
		for(int j = 0; j < 2; j++)
			printf("%lf " ,c[2*i + j]);
		printf("\n");
	}

	return 0;
}


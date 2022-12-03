#include "tools.h"

int LWORK;
double *_tmp_mm; // size m*m
double *_tmp_lwork; // size > n*m
void *_tmp_m; // ipiv ou vp_i   (size m)

static void init_prr(int n, int m, struct projection *p, struct spectre *spectre);
static void free_prr(struct projection *p);

struct spectre prr(int n, int m, double *restrict A, double *restrict x, struct prr_info *restrict prrinfo, int max_it, double _epsilon)
{
	struct projection p;
	struct spectre spectre;
	double *residu;
	double maxres;
	int count = 0;

	if(m > n){
		printf("'m' doit etre plus petit ou egal a 'n'.\n");
		exit(1);
	}
	if(max_it <= 0)
		max_it = __INT32_MAX__;

	init_prr(n,m, &p, &spectre);

	// vérification A symétrique, sinon l'algo ne s'applique pas
	for(int i = 0; i < n; i++)
		for(int j = i+1; j < n; j++)
			if(A[i * n + j] != A[j * n + i]){
				printf("'A' doit etre symmetrique.\n");
				exit(1);
			}

	normalize(x,n);
	// boucle iteration prr
	do
	{
		// algorithme PRR:
		projection(&p, A, n, m, x); // O(m * n²)
		resolution_sev(&spectre, &p,m); // O(m^3)
		retour_espace_depart(n, m, p.Vm, &spectre); // O(n * m²)
		residu = calcul_residu(n, m, A, &spectre); // O(m * n²)

		maxres = max(residu,m);
		printf("max : %lg\n", max(residu,m));

		// nouveau vecteur initial :
		get_new_x(n,m,x,residu,spectre.vec_p); // O(m * n)

		if(++count >= max_it)
			break;
	}while(maxres > _epsilon);

	free_prr(&p);
	if(prrinfo != NULL){
		prrinfo->max_residu = maxres;
		prrinfo->nb_it = count;
	}
	
	return spectre;
}

static void init_prr(int n, int m, struct projection *restrict p, struct spectre *restrict spectre)
{
	LWORK = n * (m < 4 ? 4 : m); // 4*n   ou   n*m
	_tmp_mm = malloc(m*m * sizeof(double));
	_tmp_lwork = malloc(LWORK * sizeof(double));
	_tmp_m = malloc(m * sizeof(double));
	
	p->B1 = malloc(m*m*sizeof(double));
	p->B2 = malloc(m*m*sizeof(double));
	p->Vm = malloc(LWORK*sizeof(double));

	spectre->vp = malloc(m * sizeof(double));
	spectre->vec_p = malloc(n * m * sizeof(double));
}

static void free_prr(struct projection *p)
{
	free(_tmp_mm);
	free(_tmp_lwork);
	free(_tmp_m);
	
	free(p->B1);
	free(p->B2);
	free(p->Vm);
}

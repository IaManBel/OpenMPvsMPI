# include <stdlib.h>
# include <stdio.h>
# include <omp.h>
# include "sample.h"

# ifndef MIN
# define MIN(x,y) ((x)<(y)?(x):(y))
# endif

# define DATA_SZ (1024*1024*64*3)

static double  MEM_CHUNK[DATA_SZ];

void Matrix_Init_col(int SZ, double *a, double *b, double *c){
	int j,k;

	for (k=0; k<SZ; k++) 
		for (j=0; j<SZ; j++) {
			a[j+k*SZ] = 2.0*(j+k);  
			b[k+j*SZ] = 3.2*(j-k);   
			c[j+k*SZ] = 1.0;
		}
}


int main (int argc, char **argv){
	int  N;

	if (argc < 2) {
		printf("MM1c MatrixSize [Sample arguments ...]\n"); 
		return -1;
	}

	N = (int) atof(argv[1]); argc--; argv++;

	if (N > 1024*8 | N&1){
		printf("Unvalid MatrixSize (must be multiple of 2)\n");
		return -1;
	}

	Sample_Init(argc, argv);

#pragma omp parallel
{
	int     NTHR, THR, SZ;
	int     i, j, k;
	double  *a, *b, *c;

	SZ    = N; 
	Sample_PAR_install();
	NTHR = omp_get_num_threads();
  
	a = MEM_CHUNK;
	b = a + SZ*SZ;
	c = b + SZ*SZ;

#pragma omp master
	Matrix_Init_col(SZ, a, b, c); 

	Sample_Start(THR);

#pragma omp for
	for (i=0; i<SZ; i+=2)
		for (j=0; j<SZ; j+=2) {
			double *pA, *pB, c0, c1, c2, c3;
			c0=c1=c2=c3=0.0; 
			pA = a+(i*SZ); pB = b+(j*SZ);
			for (k=SZ; k>0; k-=2, pA+=2, pB+=2){
				double a0, a1, a2, a3;
				double b0, b1, b2, b3;

				a0 = *pA;  a1 = *(pA+1);  a2 = *(pA+SZ);  a3 = *(pA+SZ+1);
				b0 = *pB;  b1 = *(pB+1);  b2 = *(pB+SZ);  b3 = *(pB+SZ+1);

				c0 += a0*b0 + a1*b1;
				c1 += a0*b2 + a1*b3;
				c2 += a2*b0 + a3*b1;
				c3 += a2*b2 + a3*b3;
			}
			pB = c+i*SZ+j;
			*pB= c0;  *(pB+1)= c1;  *(pB+SZ)= c2; *(pB+SZ+1)= c3;
		}

	Sample_Stop(THR);
	}
  
	Sample_End();
}

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

	N  = (int) atof(argv[1]); argc--; argv++;

	if (N > 1024*8){
		printf("Unvalid MatrixSize\n");
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
	for (i=0; i<SZ; i++)
		for (j=0; j<SZ; j++) {
			double *pA, *pB, c0, c1, c2, c3;
			c0=c1=c2=c3=0.0; 
			pA = a+(i*SZ); 
			pB = b+(j*SZ);
			k=SZ;
			while (k&3) { // in case SZ is not a multiple of 4
				c0 += (*pA * *pB);
				k--; pA++; pB++;
			}
			for (; k>0; k-=4, pA+=4, pB+=4) {
				c0 += (*pA * *pB);
				c1 += (*(pA+1) * *(pB+1));
				c2 += (*(pA+2) * *(pB+2));
				c3 += (*(pA+3) * *(pB+3));
			}
			c[i*SZ+j]= c0+c1+c2+c3;
		}

		Sample_Stop(THR);
	}
  
	Sample_End();
}

#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

# ifndef MIN
# define MIN(x,y) ((x)<(y)?(x):(y))
# endif

#define DATA_SZ (1024*1024*64*3)

static double  MEM_CHUNK[DATA_SZ];
double *a;
double *b;
double *c;


//Matrix Initialization
void Matrix_Init_col(int SZ, double *a, double *b, double *c){
	int j,k;

	for (k=0; k<SZ; k++)
		for (j=0; j<SZ; j++) {
			a[j+k*SZ] = 2.0*(j+k);
			b[j+k*SZ] = 3.2*(j-k);
			c[j+k*SZ] = 1.0;
		}
}//Matrix_Init


// multiply fragment of matrix
void mm1f_interval(int start, int interval, int SZ){
	int i, j, k;
  for (i=start; i<start+interval; i++)
    for (j=0; j<SZ; j++) {
      double *pA, *pB, S;
            S=0.0;
            pA = a+(i*SZ); pB = b+(j*SZ);
          for (k=SZ; k>0; k--, pA++, pB++)
            S += (*pA * *pB);
          c[i*SZ+j]= S;
      }
}//mm1c_interval



int main(int argc, char** argv){
	int rank, size, interval, remainder, i, j;
	int N  = (int) atof(argv[1]);

	a = MEM_CHUNK;
	b = a + N*N;
	c = b + N*N;

  	double time1, time2, time3;  // for timing measurements
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	MPI_Request ireq[128]; // , asynch request, assume size<128
	MPI_Status stat;       // status of asynch communication

	// compute interval size each process is responsible for,
	// rank 0 process will be responsible for the remainder
	interval = N/(size);
	remainder = N % (size);

	if (rank==0){  // root/coordinator process

		Matrix_Init_col(N, a, b, c);

		time1 = MPI_Wtime();     // record time
	      	// For array B, we broadcast the whole array, however, the Bcast
      		// operation is strange because it needs to be executed by all
	      	// processes.  There is no corresponding Recv for a Bcast
      		MPI_Bcast(a,N*N,MPI_DOUBLE,0,MPI_COMM_WORLD); // send broadcast
		// printf("%d: Bcast complete\n",rank);

		// Send intervals of array A to worker processes
		for(i=1; i<size; i++)
			MPI_Isend(a+(i*interval), interval*N, MPI_DOUBLE, i, i, MPI_COMM_WORLD, ireq+i);
		for(i=1;i<size;i++)
			MPI_Waitany(size, ireq, &j, &stat); // join on sends

		mm1f_interval(0, interval, N);                // local work
		mm1f_interval(size*interval, remainder, N);  // remainder

		//get results from workers:
		for(i=1; i<size; i++)
			MPI_Irecv(c+(i*interval), interval*N, MPI_DOUBLE, i, i, MPI_COMM_WORLD, ireq+i);
		for(i=1;i<size;i++){
			MPI_Waitany(size, ireq, &j, &stat);
			//printf("received results from process %d\n",j);
		}

		/*================= conventional MM1c ==========================*/
		time2 = MPI_Wtime();

		/*==============================================================*/

		printf("total execution time: %f \n", (time2-time1));
		}
	else {          // worker process
		MPI_Bcast(b, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD); // receive broadcast
		// synchronous receive
		MPI_Recv(a+(rank*interval), interval*N, MPI_DOUBLE,0,rank, MPI_COMM_WORLD,&stat);
		mm1f_interval(rank*interval, interval, N);
		// send results back to root process, synchronous send
		MPI_Send(c+(rank*interval), interval*N, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
	}
	MPI_Finalize();
} //main

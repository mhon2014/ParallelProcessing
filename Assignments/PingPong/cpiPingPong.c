#include "mpi.h"
#include <stdio.h>
#include <math.h>

int main( int argc, char *argv[])
{
    int numprocs, myid;
    double startTime, endTime;

    /* Initialize MPI and get number of processes and my number or rank*/
    MPI_Init(&argc,&argv);
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
	 
    if(myid==0){
    	printf("Runtime,Message Size(number of doubles)\n");
    }

    /* Processor zero sets the number of intervals and starts its clock*/

    double x[10001];

    for(int msgSize = 1; msgSize < 10001; msgSize += 500){
         startTime=MPI_Wtime();
         for (int i = 1; i <= 100; i++){

            //P0 send to P1
            if(myid==0){
                MPI_Send(x, msgSize, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
            }

            //P1 rec from P0
            if(myid==1){
                MPI_Recv(x, msgSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            }

            //P1 send to P0
            if(myid==1){
                MPI_Send(x, msgSize, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
            }

            //P0 rec from P1
            if(myid==0){
                MPI_Recv(x, msgSize, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, &status);
            } 
         }
         endTime=MPI_Wtime();
	 if(myid==0){
		printf("%.16f,%d\n",endTime-startTime, msgSize);
    	 }
    }

    MPI_Finalize();
    return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int numprocs, myid;

    double dfactor = 0.15;

    int pagenumber = 16;

    double startTime, endTime;

    if(argc > 1){
        pagenumber = atoi(argv[1]); //set if argument is present
    }

    /* Initialize MPI and get number of processes and my number or rank*/
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);

    int localrows = pagenumber / numprocs;

    int global_row;

    //initialize local rows
    double **localA = (double**) malloc(localrows * sizeof(double*));
    for (int i = 0; i < localrows; i++)
    {
        localA[i] = (double*) malloc(pagenumber * sizeof(double));
    }

    //set all to 0
    for (int i = 0; i < localrows; i++)
    {
        for (int j = 0; j < pagenumber; j++)
        {
            localA[i][j] = 0.0;
        }
    }

    for (int i = 0; i < localrows; i++)
    {
        global_row = localrows * myid + i; // from video     
        if (global_row != 0) //if global is not zero then each use the global row to set the value
        {
            localA[i][global_row - 1] = 0.5;
        }
        else                //if it is zero then set the value for the second to last index
        {
            localA[i][pagenumber - 1] = 0.5;
        }
        if (global_row != pagenumber)
        {
            localA[i][global_row + 1] = 0.5;
        }
        if (global_row == 1) //always 1
        {
            localA[i][0] = 1;
        }
    }

    int vector_size = pagenumber;
    int local_vector_size = vector_size / numprocs; //divide the task to the number of processors

    double *globalX = (double*) malloc(vector_size * sizeof(double));

    double *localX = (double*) malloc(local_vector_size * sizeof(double));

    // 1.0/N
    for (int i = 0; i < local_vector_size; i++)
    {
        localX[i] = 1.0 / (double)pagenumber;
    }

    double *globalY = (double*) malloc(vector_size * sizeof(double));

    double *localY = (double*) malloc(local_vector_size * sizeof(double));

    startTime=MPI_Wtime();

    for (int counter = 0; counter < 1000; counter++)
    {
        MPI_Allgather(localX, local_vector_size, MPI_DOUBLE,
                      globalX, local_vector_size, MPI_DOUBLE, MPI_COMM_WORLD);

        for (int i = 0; i < local_vector_size; i++)
        {
            localY[i] = 0.0;
            for (int j = 0; j < vector_size; j++)
            {
                localY[i] += (1 - dfactor) * localA[i][j] * globalX[j];
            }
            localY[i] += (dfactor / (double)pagenumber);
        }

        for (int i = 0; i < local_vector_size; i++)
        {
            localX[i] = localY[i];
        }
    }
    endTime=MPI_Wtime();

    MPI_Gather(localY, local_vector_size, MPI_DOUBLE,
               globalY, local_vector_size, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    if (myid == 0)
    {   int mini = 0, maxi = 0;
        double minvalue = globalY[0], maxvalue = minvalue;
        
        for (int i = 1; i < vector_size; i++)
        {
            if (globalY[i] >= maxvalue)
            {
                maxvalue = globalY[i];
                maxi = i;
            }
            if (globalY[i] <= minvalue)
            {
                minvalue = globalY[i];
                mini = i;
            }
            // printf("%lf\n", globalY[i]);
        }

        printf("Min: Pagerank[%d] = %lf \n", mini, minvalue);
        printf("Max: Pagerank[%d] = %lf \n", maxi, maxvalue);

        printf("Runtime = %.16e\n", endTime - startTime);
    }

    MPI_Finalize();

    return 0;
}
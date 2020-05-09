/* Jim E. Jones
   FIT Mathematics
   Math 4280
   10 February 2019 */

/* Inclusions */
#include <omp.h>      /* OpenMP */
#include <stdio.h>    /* prints */

int main (int argc, char *argv[]) 
{
  int threadCount=1;
  int threadID;

  /* Begin parallel block, each thread prints hello */

  #pragma omp parallel private(threadID)
  {
    /* Obtain thread number */
    threadID = omp_get_thread_num();
    threadCount = omp_get_num_threads();
    printf("Hello World from thread = %d\n", threadID);
  
    /* Only master thread does this print */
    #pragma omp master
      printf("Number of threads = %d\n", threadCount);
    #pragma end master`

  }  /* End of parallel block*/
}

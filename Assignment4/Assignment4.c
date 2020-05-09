#include <omp.h>    //Used for openmp
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>


long long HOTPO(long long n) {
	if (n % 2 == 0) {//If its even
		n = n / 2;
	}
	else {
		n = ((3 * n) + 1); //its odd
	}
	return n;
}

int main(int argc, char* argv[]) {
	
	//Nmax = 2000,20000,20000,2000000


	long long Nmax = 0; //Max number
	long long high = 0; //highest integer
	long long Imax = 0; //collatz

	if(argc > 1) {
		Nmax = atoi(argv[1]);
	}


	long long i = 0, j = 0, n = 0; //loop variables 
	double startTime, endTime; //Timer variables 
	int threadCount = 1, myid = 0; //Thread 

//Count Threads
#pragma omp parallel private(myid) shared(threadCount)
	{
		//thread number
		myid = omp_get_thread_num();
		printf("Thread = %d\n", myid);

		//master thread
		if (myid == 0)
		{
			threadCount = omp_get_num_threads();
			printf("Number of threads = %d\n", threadCount);
		}
	}

	//start timer
	startTime = omp_get_wtime();

	Imax = Nmax; //Assignment suggestion

	//chunk size schedule
#pragma omp parallel for private(j) schedule(dynamic,500) reduction(max:high)
	for (i = 1; i < Nmax; i++) {
		n = i;
		for (j = 1; j < Imax; j++) {
			n = HOTPO(n);
			// printf("%lld ", n);
			if (n > high) {
				high = n;
			}
			if (n == 1) {
				break;
			}
		}
	}

	printf("\nHighest value = %lld\n", high);

	//stop timer
	endTime = omp_get_wtime();

	//runtime
	printf("Runtime = %.16e\n", endTime - startTime);

	return 0;
}



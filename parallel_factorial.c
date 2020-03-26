/*
 *   Description:
 *               Calculationg factorial using OpenMP for multithreading
 *
 *   Compile:    gcc -g -Wall -fopenmp -o parallel_factorial parallel_factorial.c
 *               clang -Xpreprocessor -fopenmp -I/usr/local/include -L/usr/local/lib -lomp  parallel_factorial.c -o parallel_factorial
 * 
 *   Run:        ./parallel_factorial <thread_count> <n :(0 < n <= 20)>
 *
 *   Output:     Multithreaded factorial of n using OpenMP
 *               
 *
 *   Author:     Haya
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

void parallel_fact(int n);
void sequential_fact(int n);
int verify(int n);
void Usage(char* prog_name);

double start, elapsed;
clock_t start_clock; // for sequential time
int thread_count;
unsigned long long fact = 1; // 0 to 18,446,744,073,709,551,615 (can hold the accurate result of 20!)
/*------------------------------------------------------------------*/

int main(
    int argc, 
    char *argv[]) {
        if (argc < 3) 
            Usage(argv[0]);
        int n = strtol(argv[2], NULL, 10);
        if (n < 0 || n > 20) 
            Usage(argv[0]);
        thread_count = strtol(argv[1], NULL, 10);
        parallel_fact(n);
        printf("Parallel fact:\t\t%llu\n\t\tTotal time:\t%f milliseconds.\n", fact, (elapsed * 1000));
        //------------------------------------------ Verification for all values n <= 64
        // for (int i = 0; i <= 64; i++){
        //     if (verify(n) == 1) {
        //     printf("failed\n");
        //     exit(-1);
        //     }
        // }
        // printf("successful\n");
        return 0;
}

/*--------------------------------------------------------------------
 * Function:    parallel_fact
 * Purpose:     The parallel method for calculating the factorial
 * In arg:      n
 */
void parallel_fact(
    int n) {
        int i;
        start = omp_get_wtime();
        #   pragma omp parallel for num_threads(thread_count) \
        reduction(*: fact) 
        {
            for (i = 2; i <= n; i++) {
                fact *= i;
            }
        }
        elapsed = omp_get_wtime() - start;
} /* parallel_fact */

/*--------------------------------------------------------------------
 * Function:    sequential_fact
 * Purpose:     The serial method for calculating the factorial
 * In arg:      n
 */
void sequential_fact(
    int n) {
        //start_clock = clock();
        int i;
        for (i = 2; i <= n; i++) {
            fact *= i;
        }
        //elapsed = (double)(clock()-start_clock)/CLOCKS_PER_SEC;
} /* sequential_factorial */

/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads> <n (0 < n <= 20)>\n", prog_name);
   exit(0);
}  /* Usage */

/*-------------------------------------------------------------------
 * Function:   verify
 * Purpose:    Verifies that an array is sorted in ascending order	(Auxiliary function)
 * In args:    arr:		The array to be verified
 *             n:		The number of elements in that array
 */
int verify (
	int n) {
        fact = 1;
        sequential_fact(n);
        unsigned long int sequential_fact = fact;
        fact = 1;
        parallel_fact(n);
        unsigned long int parallel_fact = fact;
		if (sequential_fact != parallel_fact) {
            return 1;
        }
		return 0;
} /* verify */
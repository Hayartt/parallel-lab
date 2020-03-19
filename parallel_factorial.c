/*
 *   Description:
 *               Calculationg factorial using OpenMP for multithreading
 *
 *   Compile:    gcc -g -Wall -fopenmp -o parallel_factorial parallel_factorial.c
 *               clang -Xpreprocessor -fopenmp -I/usr/local/include -L/usr/local/lib -lomp  parallel_factorial.c -o parallel_factorial
 * 
 *   Run:        ./parallel_factorial <thread_count> <n :(0 < n < 65)>
 *
 *   Output:     The factorial of n using the parallel method and the sequential method.
 *               
 *
 *   Author:     Haya
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void parallel_fact(int n);
void sequential_fact(int n);
int verify(int n);
void Usage(char* prog_name);

int thread_count;
unsigned long int fact = 1; // 0 to 4,294,967,295 (can hold the result of 64!)
/*------------------------------------------------------------------*/

int main(
    int argc, 
    char *argv[]) {
        if (argc < 3) 
            Usage(argv[0]);
        int n = strtol(argv[2], NULL, 10);
        if (n < 0 || n > 64) 
            Usage(argv[0]);
        thread_count = strtol(argv[1], NULL, 10);
        parallel_fact(n);
        printf("Parallel fact: %lu\n", fact);
        fact = 1;
        sequential_fact(n);
        printf("Sequential fact: %lu\n", fact);
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
        #   pragma omp parallel for num_threads(thread_count) \
        reduction(*: fact) 
        {
            for (i = 2; i <= n; i++) {
                fact *= i;
            }
        }
} /* parallel_fact */

/*--------------------------------------------------------------------
 * Function:    sequential_fact
 * Purpose:     The serial method for calculating the factorial
 * In arg:      n
 */
void sequential_fact(
    int n) {
        int i;
        for (i = 2; i <= n; i++) {
            fact *= i;
        }
} /* sequential_factorial */

/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads> <n (0 < n < 65)>\n", prog_name);
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
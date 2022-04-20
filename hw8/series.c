#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include "unixtimer.h"

#define DEFAULT_N  1000000
#define DEFAULT_NTHREADS 2
#define even(x) ((x) % 2 == 0)

///////////////////////////////////////////////////////////////////////

typedef struct thread_result {
    int n_per_thread;
    int sum;
} thread_result;

///////////////////////////////////////////////////////////////////////

// Define any necessary macros, types, and additional functions here
// TODO

void* run_sim(void* arg) {
    thread_result* result = (thread_result*) arg;
    double limit = result->n_per_thread;
    double sum = 0;
    for (int i = 0; i < limit; i++) {
        if (even(i)) sum += 1.0 / (2 * i + 1);
        else sum -= 1.0 / (2 * i + 1);
    }

    printf("%d\n", sum);
    printf("%d\n", result->sum);    
    result->sum += sum;
    return NULL;
}

/////////////////////////////////////////////////////////////////
//
// Compute the sum of first n terms of the Madhava-Leibniz series
// using num_threads threads
//
double sum_threads(long n, int num_threads) {
    pthread_t threads[num_threads];
    thread_result result;
    result.n_per_thread = n / num_threads;

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, &run_sim, (void*) &result);
    }

    double sum = 0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        sum += result.sum;
    }

    printf("Sum = %f\n", sum);
    return sum;
}


/////////////////////////////////////
// STUDY BUT DO NOT MODIFY CODE BELOW

int main(int argc, char *argv[])
{
   long n           = (argc < 2 ? DEFAULT_N : atoi(argv[1]) );
   int  num_threads = (argc < 3 ? DEFAULT_NTHREADS : atoi(argv[2]) );
   double PI25 = 3.141592653589793238462643;

   // Compute and print the approximation of pi
   start_clock();
   start_timer();
   double my_pi = 4 * sum_threads(n, num_threads);
   printf("pi approximation: %.16f Error: %.16f\n", my_pi, fabs(my_pi - PI25));
   printf("Clock time = %.2f CPU time =  %.2f\n", clock_seconds(), cpu_seconds());

   return 0;
}
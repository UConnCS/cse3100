#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#define     DEFAULT_DARTS_PER_THREAD 500000
#define     DEFAULT_NUM_THREADS           2

/* Using TLS for the random number generator buffer to avoid data races between threads
   */
__thread unsigned short buff[3];

/*
   Call the following macro in your thread function before any calls to RANDOM_DOUBLE()
   By seeding the random number generator based on the thread ID each thread gets a different
   seed, and the seeds are different in different program runs.
   */
#define RANDOM_INIT()  (buff[0]=buff[1]=buff[2]=(unsigned short)pthread_self())

/*
   RANDOM_DOUBLE() returns a random number uniformly distributed between $[0, 1)$
   */
#define RANDOM_DOUBLE() (erand48(buff))

typedef struct thread_result {
    int darts_per_thread;
    int darts_thrown;
} thread_result;

///////////////////////////////////////////////////////////////////////

// Define any necessary macros, types, and additional functions here
// TODO

void* run_sim(void* arg) {
    thread_result* result = (thread_result*) arg;
    double limit = result->darts_per_thread;

    double x, y;
    double darts = 0;
    for (int i = 0; i < limit; i++) {
        x = RANDOM_DOUBLE();
        y = RANDOM_DOUBLE();
        if (x*x + y*y < 1)
            darts += 1;
    }

    result->darts_thrown += darts;
    return NULL;
}

int main(int argc, char ** argv)
{
    int     num_threads      = DEFAULT_NUM_THREADS;
    int     darts_per_thread = DEFAULT_DARTS_PER_THREAD;

    if (argc > 2) {
        num_threads = atoi(argv[1]);
        darts_per_thread = atoi(argv[2]);
        assert( num_threads>0 && darts_per_thread>0 );
    }
    else {
        fprintf(stderr, "Usage: %s <num_threads> <darts_per_thread>\n", argv[0]);
        fprintf(stderr, "Currently using the default values\n");
    }

    long long n = (long long)num_threads * darts_per_thread;

    printf("Throwing %lld darts using %d thread%s\n", n, num_threads, (num_threads>1 ? "s" : "") );

    // Add code to create num_threads threads, each simulating the twrowing of
    // darts_per_thread darts at the [0, 1) × [0, 1) square.  Compute in p the
    // number of darts that fall within the circle of radius 1 centered at (0,0)

    pthread_t threads[num_threads];
    thread_result result;
    result.darts_per_thread = darts_per_thread;

    double p = 0;
    for (int i = 0; i < num_threads; i++) {        
        pthread_create(&threads[i], NULL, run_sim, (void*) &result);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        p += result.darts_thrown;
    }

    // DO NOT MODIFY THE CODE BELOW
    // estimate pi and compare with M_PI, the value defined in math.h
    double my_pi = (double)p / n * 4.0;
    printf("Pi approximation: %.16f Error: %.16f\n", my_pi, fabs(my_pi - M_PI));
    return 0;
}
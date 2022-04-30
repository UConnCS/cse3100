#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_barrier_t barrier;
pthread_spinlock_t spinlock;

// the thread that prints "Hakuna "
void* print_Hakuna(void* arg)
{
    long num_copies = *(long*)arg;
    for (int i = 0; i < num_copies; i++) {
        pthread_spin_lock(&spinlock);
        printf("Hakuna ");
        pthread_spin_unlock(&spinlock);
        pthread_barrier_wait(&barrier);
        pthread_barrier_wait(&barrier);
    }
    pthread_exit(NULL);
}

// the thread that prints "Matata\n"
void* print_Matata(void* arg)
{
    long num_copies = *(long*)arg;
    for (int i = 0; i < num_copies; i++) {
        pthread_spin_lock(&spinlock);
        printf("Matata\n");
        pthread_spin_unlock(&spinlock);
        pthread_barrier_wait(&barrier);
        pthread_barrier_wait(&barrier);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    int ret1, ret2;
    pthread_t tid1, tid2;

    if(argc < 2) {
        fprintf(stderr, "Usage: %s <num_copies>\n", argv[0]);
        exit(1);
    }
    
    long num_copies = atol(argv[1]);
    pthread_barrier_init(&barrier, NULL, 2);
    pthread_spin_init(&spinlock, 0);

    ret1 = pthread_create(&tid1, NULL, print_Hakuna, &num_copies);
    ret2 = pthread_create(&tid2, NULL, print_Matata, &num_copies);
    if (ret1 || ret2) {
        fprintf(stderr, "ERROR: could not create threads\n");
        exit(-1);
    }

    ret1 = pthread_join(tid1, NULL);
    ret2 = pthread_join(tid2, NULL);

    if (ret1 || ret2) {
        fprintf(stderr, "ERROR: could not join threads\n");
        exit(-1);
    }

    pthread_barrier_destroy(&barrier);
    pthread_spin_destroy(&spinlock);
    
    return 0;
}
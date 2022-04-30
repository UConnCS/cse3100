#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX 28

pthread_barrier_t bar;

void *bizz(void* arg) {
    int* ip = arg;
    while (*ip < MAX) {
        if ( (*ip % 2 == 0) && (*ip % 3 != 0))
            printf("bizz\n");
        pthread_barrier_wait(&bar);
        pthread_barrier_wait(&bar);
    }
    pthread_exit(NULL);
}

void* bazz(void* arg) {
    int* ip = arg;
    while (*ip < MAX) {
        if ( (*ip % 2 != 0) && (*ip % 3 == 0))
            printf("bazz\n");
        pthread_barrier_wait(&bar);
        pthread_barrier_wait(&bar);
    }
}

void* bizzbazz(void* arg) {
    int* ip = arg;
    while (*ip < MAX) {
        if ( (*ip % 2 == 0) && (*ip % 3 == 0))
            printf("bizzbazz\n");
        pthread_barrier_wait(&bar);
        pthread_barrier_wait(&bar);
    }
}

int main() {
    int i = 8;
    int* ip = &i;
    pthread_t tid;

    pthread_barrier_init(&bar, NULL, 4);
    pthread_create(&tid, NULL, bizz, ip);
    pthread_create(&tid, NULL, bazz, ip);
    pthread_create(&tid, NULL, bizzbazz, ip);

    while (*ip < MAX) {
        if ( (*ip % 2 != 0) && (*ip % 3 != 0))
            printf("%d\n", *ip);
        pthread_barrier_wait(&bar);
        (*ip)++;
        pthread_barrier_wait(&bar);
    }

    return 0;
}
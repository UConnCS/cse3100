#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#define ITERATIONS 10
#define DEFAULT_NFEMALES 10
#define DEFAULT_NMALES 10
#define DEFAULT_NSTALLS 2

__thread unsigned short buff[3];

#define RANDOM_INIT() (buff[0] = buff[1] = buff[2] = (unsigned short)pthread_self())

#define WORK() msleep(nrand48(buff) % 50)

#define USE_BATHROOM() msleep(nrand48(buff) % 10)

#define isFull(bathroom) (bathroom->num_males + bathroom->num_females == bathroom->num_stalls)

void msleep(int tms) {
    struct timeval tv;
    tv.tv_sec = tms / 1000;
    tv.tv_usec = (tms % 1000) * 1000;
    select(0, NULL, NULL, NULL, &tv);
}

typedef struct bathroom {
    int num_females;
    int num_males;
    int num_stalls;
    pthread_mutex_t mutex;
    pthread_cond_t available;
} bathroom_t;

typedef struct thread_result {
    int id;
    bathroom_t *bathroom;
} thread_result;

void* thread_func_female(void *arg) {
    thread_result *data = (thread_result *)arg;
    bathroom_t *bathroom = data->bathroom;
    int id = data->id;

    RANDOM_INIT();

    for (int i = 0; i < ITERATIONS; i++) {
        WORK();
        pthread_mutex_lock(&bathroom->mutex);

        while (bathroom->num_males > 0 || isFull(bathroom)) {
            printf("female #%d waits\n", id);
            pthread_cond_wait(&bathroom->available, &bathroom->mutex);
        }

        bathroom->num_females++;
        printf("female #%d enters bathroom\n", id);
        pthread_mutex_unlock(&bathroom->mutex);
        USE_BATHROOM();

        pthread_mutex_lock(&bathroom->mutex);
        bathroom->num_females--;
        
        pthread_cond_broadcast(&bathroom->available);
        printf("female #%d exits bathroom\n", id);
        pthread_mutex_unlock(&bathroom->mutex);
    }

    pthread_exit(NULL);
}

void *thread_func_male(void *arg) {
    thread_result *data = (thread_result *)arg;
    bathroom_t *bathroom = data->bathroom;
    int id = data->id;

    RANDOM_INIT();

    for (int i = 0; i < ITERATIONS; i++) {
        WORK();
        pthread_mutex_lock(&bathroom->mutex);

        while (bathroom->num_females > 0 || isFull(bathroom)) {
            printf("male #%d waits\n", id);
            pthread_cond_wait(&bathroom->available, &bathroom->mutex);
        }

        bathroom->num_males++;
        printf("male #%d enters bathroom\n", id);
        pthread_mutex_unlock(&bathroom->mutex);
        USE_BATHROOM();

        pthread_mutex_lock(&bathroom->mutex);
        bathroom->num_males--;
        
        pthread_cond_broadcast(&bathroom->available);
        printf("male #%d exits bathroom\n", id);
        pthread_mutex_unlock(&bathroom->mutex);
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int status;
    int num_females = DEFAULT_NFEMALES;
    int num_males = DEFAULT_NMALES;
    int num_stalls = DEFAULT_NSTALLS;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-f", strlen("-f")) == 0 && i + 1 < argc) {
            num_females = atoi(argv[++i]);
        } else if (strncmp(argv[i], "-m", strlen("-m")) == 0 && i + 1 < argc) {
            num_males = atoi(argv[++i]);
        } else if (strncmp(argv[i], "-s", strlen("-s")) == 0 && i + 1 < argc) {
            num_stalls = atoi(argv[++i]);
        } else {
            fprintf(stderr, "Usage: %s [-f N|-females N] [-m N|-males N] [-s N|-stalls N]\n", argv[0]);
            return 1;
        }
    }

    bathroom_t bathroom;
    bathroom.num_females = bathroom.num_males = 0;
    bathroom.num_stalls = num_stalls;
    pthread_mutex_init(&bathroom.mutex, NULL);
    pthread_cond_init(&bathroom.available, NULL);
    
    int total = num_females + num_males;
    pthread_t threads[total];
    thread_result results[total];

    for (int i = 0; i < num_females; i++) {
        thread_result res;
        res.bathroom = &bathroom;
        res.id = i;
        results[i] = res;
        status = pthread_create(&threads[i], NULL, thread_func_female, &results[i]);
        
        assert(status == 0);
    }

    for (int i = num_females; i < total; i++) {
        thread_result res;
        res.bathroom = &bathroom;
        res.id = i - num_females;
        results[i] = res;
        status = pthread_create(&threads[i], NULL, thread_func_male, &results[i]);
        
        assert(status == 0);
    }

    for (int i = 0; i < total; i++)
        pthread_join(threads[i], NULL);

    return 0;
}
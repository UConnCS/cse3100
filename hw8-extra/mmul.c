#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include "square_matrix.h"

///////////////////////////////////////////////////////////////////////

typedef struct thread_result {
    matrix_element** a;
    matrix_element** b;
    matrix_element** result;
    size_t start;
    size_t end;
    size_t len;
} thread_result;

void* matrix_product_thread(void* arg) {
    thread_result* res = (thread_result*) arg;
    size_t start = res->start;
    size_t end = res->end;
    size_t len = res->len;

    matrix_element** a = res->a;
    matrix_element** b = res->b;
    matrix_element** result = res->result;

    for (size_t i = start; i < end; i++) {
        for (size_t j = 0; j < len; j++) {
            result[i][j] = 0;
            for (size_t k = 0; k < len; k++) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

/*
 * Compute the sum of two square matrices. Return a pointer to the
 * newly allocated result matrix or NULL if anything is wrong
 *
 * Similar to add_square_matrices() in square_matrix.c but using multi-threading
 */
square_matrix* mul_square_matrices_threads(square_matrix *m1, square_matrix *m2, size_t num_threads) {
    pthread_t threads[num_threads];
    thread_result results[num_threads];
    size_t len = m1->order;

    matrix_element** a = m1->data;
    matrix_element** b = m2->data;
    square_matrix* result = new_square_matrix(len);
    matrix_element** data = result->data;

    for (size_t i = 0; i < num_threads; i++) {
        size_t start = i * len / num_threads;
        size_t end = i == num_threads - 1
            ? len
            : (i + 1) * len / num_threads;

        results[i].a = a;
        results[i].b = b;
        results[i].result = data;
        results[i].start = start;
        results[i].end = end;
        results[i].len = len;

        pthread_create(&threads[i], NULL, &matrix_product_thread, &results[i]);
    }

    for (size_t i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    result->data = data;
    return result;
}

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "square_matrix.h"

///////////////////////////////////////////////////////////////////////
// Define any necessary macros, types, and additional functions here
typedef struct thread_result {
    matrix_element** m1;
    matrix_element** m2;
    matrix_element** res_data;
    size_t start;
    size_t end;
    size_t len;
} thread_result;

/*
 * Transpose a square matrix
 *
 * Similar to transpose_square_matrix, but with multi-threading.
 */
void* transpose_thread(void* arg) {
    thread_result* data = (thread_result*) arg;
    matrix_element** m1 = data->m1;
    matrix_element** res_data = data->res_data;

    size_t len = data->len;
    size_t start = data->start;
    size_t end = data->end;
    
    for (size_t i = start; i < end; i++) {
        for (size_t j = 0; j < len; j++) {
            res_data[j][i] = m1[i][j];
        }
    }
    
    pthread_exit(NULL);
}
    
    
square_matrix* transpose_square_matrix_threads(square_matrix* m, size_t num_threads) {
    if (m == NULL)
        return NULL;
    
    pthread_t threads[num_threads];
    thread_result results[num_threads];

    size_t len = m->order;
    square_matrix *res = new_square_matrix(len);
    matrix_element **data = m->data;
    matrix_element **res_data = res->data;
    
    for (size_t i = 0; i < num_threads; i++){
        long start = (i * len) / num_threads;
        long end = ((i + 1) * len) / num_threads;

        thread_result res;
        res.m1 = data;
        res.res_data = res_data;
        res.start = start;
        res.end = end;
        res.len = len;
        results[i] = res;

        pthread_create(&threads[i], NULL, &transpose_thread, &results[i]);
    }

    for (size_t i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);
    
    return res;
}
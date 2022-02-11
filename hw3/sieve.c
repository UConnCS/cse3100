#include <stdio.h>
#include <stdlib.h>

/*
 * Use the sieve of Erathostenes to find all primes from 2 up to a given number max
 * Input:
 *    max: the upper bound of the range of integers in which primes must be searched
 *    primes: pointer to an int array (to be allocated within sieve) that holds the
 *            primes in increasing order when the function returns
 *  Return:
 *    the number of primes found
 */

void fill_array(int* arr, int max) {
    for (int i = 2; i <= max; i++)
        arr[i] = i;
}

void bulk_invalidate(int* arr, int max) {
    for (int i = 2; i <= max; i++) {
        if (arr[i] == 0) {
            continue;
        }

        for (int j = i * 2; j <= max; j += i) {
            arr[j] = 0;
        }
    }
}

int sieve(int max, int* primes) {
    if (max < 2) {
        return 0;
    }

    if (max == 2) {
        primes[0] = 2;
        return 1;
    }

    if (max == 3) {
        primes[0] = 2;
        primes[1] = 3;
        return 2;
    }
    
    int* res = (int*) malloc((max + 1) * sizeof(int));
    
    fill_array(res, max);
    bulk_invalidate(res, max);

    int size = 0;
    for (int i = 0; i < max; i++) {
        if (res[i] != 0)
            size++;
    }

    int j = 0;
    for (int i = 0; i < max; i++) {
        if (res[i] == 0)
            continue;

        primes[j] = res[i];
        j++;
    }

    free(res);
    return size;
}

// DO NOT MODIFY CODE BELOW

/*
 * compute polynomial hash code with base 257 for a given array of unsigned chars
 */
#define PRIME_BASE 257u
unsigned int hash( unsigned char s[], int size) {
    unsigned int result = 0;

    // compute polynomial hash code using Horner's rule
    for( int i=size-1; i>=0; i-- )
        result = s[i] + PRIME_BASE * result;

    return result;
}

int main(void) {
    int max;
    scanf("%d", &max);

    int* primes = (int*) malloc(max * sizeof(int));  // pointer to the prime array
    int n_primes = sieve(max, primes);

    if (n_primes > 0)
        printf("%d primes with checksum %u\n", n_primes,
                hash((unsigned char*) primes, n_primes * sizeof(int)));

    // free the primes array
    free(primes);

    return 0;
}
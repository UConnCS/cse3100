#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* This program should print the sum of the first n positive cubes
 *     1^3 + 2^3 + 3^3 + ... + n^3
 * where n is a positive integer read from the input
 * Hunt down the bugs and squash them!
 * Seek out the memory leaks and plug them up!
 */

/* Computes the sum of the first n elements in the array. */
int sum(int n, int arr[]) {
    int total = 0;
    for (int i = 0; i < n; i++)
        total += arr[i];
    return total;
}

/* Fills the given array with the values
 * 1^3, 2^3, 3^3, ..., (n-1)^3, n^3.
 */
void fillCubes(int n, int arr[]) {
    for (int i = 0; i < n; i++) {
        int k = i + 1;
        arr[i] = k * k * k;
    }
}

/* Takes a positive integer n from the command line,
 * fills array with first n cubes, then computes
 * the sum of the cubes. Prints out the
 * sum before freeing all used memory.  If no command
 * line argument is given prints a message on how
 * the program should be run.
 */
int main(int argc, char* argv[]) {
    int* arr;

    if (argc < 2) {
        printf("usage: %s n\n", argv[0]);
        return 1;
    }

    // convert the first argument from a string to a number
    int n = atoi(argv[1]);
    assert(n > 0);

    arr = malloc(n * sizeof(int));

    fillCubes(n, arr);
    int total = sum(n, arr);
    printf("total: %d\n", total);

    free(arr);
    return 0;
}

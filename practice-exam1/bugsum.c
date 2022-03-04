#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        printf("usage: %s n\n", argv[0]);
        return 0;
    }

    int n = atoi(argv[1]);
    int* A = malloc(sizeof(int) * n);
    int sum = 0;

    for (int i = n; i > 0; i--)
        A[i - 1] = i;

    for (int i = 0; i < n; i++)
        sum += A[i];

    printf("sum=%d\n", sum);
    free(A);

    return 0;
}
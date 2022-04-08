#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    fork();
    printf("Hello\n");
    fflush(stdout);
    fork();
    fork();
    printf("Hello\n");
    fflush(stdout);
}
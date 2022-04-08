#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int n, status;
    n = 10;
    printf("A: %d\n", n);
    fflush(stdout);
    pid_t pid = fork();
    if (pid == 0) {
        n = 42;
        printf("B: %d\n", n);
        fflush(stdout);
    } else if (pid > 0) {
        wait(&status);
    }
    printf("C: %d\n", n);
    fflush(stdout);
}
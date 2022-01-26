#include <stdio.h>

// Take any positive integer n. If n is even, divide n by 2 to get n / 2. If n is odd, multiply n by 3 and add 1 to get 3n + 1. Repeat the process indefinitely.  The conjecture states that no matter which number you start with, you will always reach 1 eventually.

int _collatz(int n, int steps) {
    if (n == 1)
        return steps;

    if (n % 2 == 0)
        return _collatz(n / 2, steps + 1);
    
    return _collatz((n * 3) + 1, steps + 1);
}

int collatz_steps(int n) {
    return _collatz(n, 0);
}

int main() {
    printf("%d=9\n", collatz_steps(12));
    printf("%d=0\n", collatz_steps(1));
}
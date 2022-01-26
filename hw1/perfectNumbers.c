#include <stdio.h>

int perfect_number(int n) {
    int result = 0;
    for (int i = 1; i < n; i++) {
        if (n % i == 0)
            result += i;
    }

    return result == n;
}

int main() {
    printf("%d=1\n", perfect_number(6));
    printf("%d=0\n", perfect_number(8));
    printf("%d=1\n", perfect_number(28));
    printf("%d=0\n", perfect_number(12));
    printf("%d=0\n", perfect_number(1));
}
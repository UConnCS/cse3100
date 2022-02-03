#include <stdio.h>

int power1(int b, int n) {
    int rv = 1;

    while (n>0) {
        rv *= b;
        n--;
    }

    return rv;
}

int power2(int b, int n) {
    int rv = 1;
    while (n--)
        rv *= b;
    return rv;
}

int power(int b, int n) {
    if (n == 0) return 1;
    return b * power(b,n-1);
}

int main() {
    printf("%d", power(2, -1));
    printf("%d", power1(2, -1));
    printf("%d", power2(2, -1));
}
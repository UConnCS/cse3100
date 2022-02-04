#include <stdio.h>

int main() {
    int i, odd, even;

    // count odd and even integers between 0 and 199
    for (i = odd = even = 0; i < 200; i++)
        if (i & 1) {
            odd++;
        }
        else {
            even++;
        }
    printf("odd: %d, even: %d\n", odd, even);
    return 0;
}
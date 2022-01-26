#include <stdio.h>

int longest_run(long input) {
    int a[32];

    long n = input < 0
        ? -input
        : input;

    // init array with zeroes
    for (int i = 0; i < 32; i++)
        a[i] = 0;

    // pop off each bit and add to array
    for (int i = 0; n > 0; i++) {
        a[i] = n % 2;
        n /= 2;
    }

    int bits[32];

    // reverse array to get actual bin representation
    for (int i = 31, j = 0; i >= 0; i--, j++)
        bits[j] = a[i];

    // if result is < 0, perform signed conversion
    // since original bits are unsigned
    if (input < 0) {
        // one's compliment, flip all bits
        for (int i = 0; i < 32; i++)
            bits[i] = bits[i] == 0
                ? 1
                : 0;

        // two's compliment, add one
        int add = 1;
        for (int i = 31; i >= 0; i--) {
            int bit = bits[i];
            if (bit == 1 && add == 1) {
                bit = 0;
            } else if (bit == 0 && add == 1) {
                bit = 1;
                add = 0;
            }

            bits[i] = bit;
        }
    }

    int count = 0;
    int longest = 0;

    for (int i = 0; i < 32; i++) {
        int bit = bits[i];
        if (bit == 1)
            count += 1;
        else
            count = 0;

        if (count > longest)
            longest = count;
    }

    return longest;
}

int main() {
    printf("%d=1\n", longest_run(1));
    printf("%d=1\n", longest_run(2));
    printf("%d=32\n", longest_run(-1));
    printf("%d=0\n", longest_run(0));
    printf("%d=6\n", longest_run(669243533));
    printf("%d=5\n", longest_run(61668212));
    printf("%d=5\n", longest_run(558227253));
    printf("%d=2\n", longest_run(1079084425));
    printf("%d=1\n", longest_run(-2147483648));
}
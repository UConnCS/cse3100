/**
 * Write a function circular_shift() that takes as arguments an integer a and an unsigned integer pos, and returns the integer obtained from a by performing a circular left-shift of its bits by pos positions.
 * Unlike the C left shift operator, which drops the high order bits, the circular left shift must insert the high order bits as the low order bits of the result. 
 */

#include <math.h>
#include <stdio.h>

long circular_shift(unsigned int a, int pos) {
    return a << pos | (a >> (32 - pos));
}

int main() {
    printf("\n%ld=2\n", circular_shift(1, 1));
    printf("\n%ld=1024\n", circular_shift(1, 10));
    printf("\n%ld=1\n", circular_shift(1, 1024));
}
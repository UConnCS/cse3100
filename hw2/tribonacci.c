#include <stdio.h>
#include <stdlib.h>

/*
 *  Return the sum of even tribonacci numbers between 1 and max
 */
long tribonacci(long n, long a, long b, long c) {
    if (n == 0 || n == 1)
        return 0;

    if (n == 2)
        return c;

    return tribonacci(n - 1, b, c, a + b + c);
}

long even_tribonacci_sum(long max) {
    unsigned long sum = 0;
    for (long i = 0; i < max; i++) {
        unsigned long result = tribonacci(i, 0, 0, 1);
        if ((result & 1) != 0)
            continue;
        
        if (result > max)
            break;
        
        sum += result; 
    }
    
    return sum;
}

// DO NOT MODIFY THE CODE BELOW

int main(void) {
    long max;

    scanf("%ld", &max);
    printf("%ld\n", even_tribonacci_sum(max));

    return 0;
}
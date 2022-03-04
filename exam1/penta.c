#include <stdio.h>

/**
 * Pentanacci numbers are a generalization of the Fibonacci numbers.  The first five Pentanacci numbers are:
 * P1 = 0, P2 = 0, P3 = 0, P4 = 1, P5 = 2
 * 
 * For n>5, the n-th Pentanacci number is given by
 * 
 * Pn = Pn-1 + 2*Pn-2 + Pn-3 + 2*Pn-4 + 3*Pn-5
 * 
 * Complete the implementation of the even_pentanacci_sum() function which takes as input a long max and returns the sum of all even Pentanacci numbers between 0 and max. 
 */

long even_pentanacci_sum(long max) {
    
}

// DO NOT MODIFY THE CODE BELOW

int main( void )
{
    long max;

    scanf("%ld", &max);

    printf( "%ld\n", even_pentanacci_sum( max ) );

    return 0;
}

#include <stdio.h>

// Recall that a leap year in the Gregorian calendar occurs on every year that is evenly divisible by 4, except every year that is evenly divisible by 100, unless the year is also evenly divisible by 400.

int leap_year(int n) {
    if (n % 4 != 0)
        return 0;

    if (n % 100 == 0 && n % 400 != 0)
        return 0;

    return 1;
}

int main() {
    printf("%d=0\n", leap_year(1997));
    printf("%d=1\n", leap_year(1996));
    printf("%d=0\n", leap_year(1900));
    printf("%d=1\n", leap_year(2000));
}
#include <stdio.h>
#include <math.h>

int main() {
    int test = pow(2, 31);
    printf("%d\n", test);
    test += 2;
    printf("%d\n", test);
    return 0;
}
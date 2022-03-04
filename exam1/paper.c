#include <stdio.h>

int main() {
    char* message = "C is fun";
    printf("%s\n", message + 2);

    int a[5] = {22,33,44,55,66};
    int* ptr = a;
    int res = *(ptr + 2);
    printf("%d\n", res);

    int i = 0;
    printf("%d\n", i++ && ++i);

    char* a = "abc";
    char* b = "def";

    // *--a = *b++;
    printf("%s\n", a);
}
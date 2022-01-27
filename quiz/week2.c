#include <assert.h>
#include <string.h>
#include <stdio.h>

#define typename(x) _Generic((x), \
    int:     "int", \
    float:   "float", \
    double:  "double", \
    short:   "short", \
    long:    "long", \
    default: "other")

int main() {
    short s;
    int i;
    long l;
    float f;
    double d;

    printf("f+s=%s\n", typename(f+s));
    printf("l/d=%s\n", typename(l/d));
    printf("f/l+i=%s\n", typename(f/l+i));
    printf("l*s=%s\n", typename(l*s));
    printf("f/2=%s\n", typename(f/2));
    printf("i/(d+2)=%s\n", typename(i/(d+2)));
    printf("l/(i*2.0)=%s\n", typename(l/(i*2.0)));
    printf("s++=%s\n", typename(s++));
    printf("s+1=%s\n", typename(s+1));
    printf("d++=%s\n", typename(d++));
}
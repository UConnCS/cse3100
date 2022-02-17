#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* return the length of a string
 * For example,
 * return 0 if s is ""
 * return 3 if s is "abc"
 */
unsigned long my_strlen(char *s)
{
    unsigned long    i = 0;

    while (*s++)
        i++;
    return i;
}

/* Concatnate two strings.
 * Dynamically allocate space for the result.
 * Return the address of the result.
 */
char *my_strcat(char *s1, char *s2)
{
    char *r;
    unsigned long len1, len2, i;

    len1 = my_strlen(s1);
    len2 = my_strlen(s2);

    r = malloc(len1 + len2 + 1);
    assert (r != NULL);

    // copy, using array notation
    for(i = 0; i < len1; i++)
        r[i] = s1[i];

    for(i = 0; i < len2; i++)
        r[len1+i] = s2[i];

    r[len1+len2] = 0;
    return r;
}

/*
 * Concatenate command line arguments in reverse order
 * and print them to the standard output
 */
int main(int argc, char *argv[])
{
    int     i;
    char    *s;

    // initialize s to be ""
    s = malloc(1);
    assert(s != NULL);
    *s = 0;

    for(i = argc-1; i >= 0; i--)
        s = my_strcat(s, argv[i]);

    printf("%s\n", s);

    return 0;
}
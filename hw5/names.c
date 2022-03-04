#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void change_word(char* str, int len) {
    for (int i = 0; i < len; i++)
        str[i] = toupper(str[i]);
}

int capitalize_str(char* str, const char* name) {
    int hits = 0;
    int len = strlen(str);
    int len_name = strlen(name);
    int lastpos = len - len_name;
    for (int n = 0; n <= lastpos; n++) {
        if (strncasecmp(str + n, name, len_name) == 0) {
            change_word(str + n, len_name);
            hits++;
            n += (len_name - 1);
         }
     }
     return hits;
}

int main(int argc, char* argv[]) {
    int hits_total=0;
    char *mycopy;

    if (argc < 3)
        return 1;
    
    mycopy = strdup(argv[1]);
    
    for (int na = 2; na < argc; na++)
        hits_total += capitalize_str(mycopy, argv[na]);
    
    printf("Total %d hits:\n", hits_total);
    printf("original: %s\n", argv[1]);
    printf("modified: %s\n", mycopy);
    free(mycopy);
    
    return 0;
}
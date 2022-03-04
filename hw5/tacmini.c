#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // read input from either stdin or argv[1] if provided
    FILE *fp = NULL;
    if (argc == 2) {
        fp = fopen(argv[1], "r");
    }

    int std = 0;
    if (argc == 1) {
        FILE* out = fopen("./stdin.txt", "w");

        // store stdin into input
        fp = stdin;
        char* input = NULL;
        size_t len = 0;
        while (getline(&input, &len, fp) != -1) {
            fprintf(out, "%s", input);
        }

        // free memory
        free(input);
        fclose(out);
        std = 1;
        fp = fopen("./stdin.txt", "r");
    }

    if (fp == NULL) {
        printf("Error: Couldn't open file.\n");
        return 1;
    }

    // count file lines
    int lc = 0;
    char line[256];
    while (fgets(line, sizeof(lc), fp)) {
        lc++;
    }

    if (lc == 0)
        return 0;

    // read file again
    fseek(fp, 0, SEEK_END);

    while (ftell(fp) > 1) {
        fseek(fp, -2, SEEK_CUR);
        char c = fgetc(fp);
        int i = 0;

        while (c != '\n') {
            line[i] = c;
            i++;

            if (ftell(fp) < 2)
                break;

            fseek(fp, -2, SEEK_CUR);
            c = fgetc(fp);
        }

        for (int j = i - 1; j >= 0; j--) {
            printf("%c", line[j]);
        }

        printf("\n");
    }

    if (std) remove("./stdin.txt");
    fclose(fp);
    return 0;
}
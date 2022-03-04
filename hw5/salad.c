#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    int files = argc - 1;
    FILE** fp = malloc(files * sizeof(FILE*));

    for (int i = 0; i < files; i++) {
        fp[i] = fopen(argv[i + 1], "r");
        assert(fp[i] != NULL);
    }

    int* done = malloc(files * sizeof(int));
    for (int i = 0; i < files; i++) {
        done[i] = 0;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while (1) {
        for (int i = 0; i < files; i++) {
            if (done[i]) {
                printf("\n");
                continue;
            }
            
            if ((read = getline(&line, &len, fp[i])) == -1) {
                done[i] = 1;
                printf("\n");
                continue;
            }

            printf("%s", line);
        }

        int done_all = 1;
        for (int i = 0; i < files; i++) {
            if (!done[i]) {
                done_all = 0;
                break;
            }
        }
        
        if (done_all) {
            break;
        }
    }

    for (int i = 0; i < files; i++) {
        fclose(fp[i]);
    }
    
    free(fp);
    free(line);
    free(done);

    return 0;
}

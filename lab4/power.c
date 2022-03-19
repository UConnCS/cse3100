#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <assert.h>

int main(int argc, char** argv) {
    char *cmd = "/opt/homebrew/bin/python3";
    int in_fd[2];
    int out_fd[2];

    pipe(in_fd);
    pipe(out_fd);

    // fd[0] is the read end of the pipe
    // fd[1] is the write end of the pipe

    pid_t pid = fork();
    if (pid == 0) {
        // read stdin from parent
        close(0);
        dup2(in_fd[0], 0);

        // redirect stdout to parent
        close(1);
        dup2(out_fd[1], 1);

        // redirect stderr to /dev/null
        close(2);

        // exec python -i
        execl(cmd, cmd, "-i", NULL);

        // get the error message
        char *err_msg = strerror(errno);
        fprintf(stderr, "failed to execute %s: %s\n", cmd, err_msg);
    } else {
        char buf[1024];
        while (fgets(buf, sizeof(buf), stdin) != NULL) {
            int i, j;
            if (sscanf(buf, "%d %d", &i, &j) != 2)
                return 1;

            // create the string "i ** j"
            char* payload = malloc(sizeof(char) * (strlen(buf) + 1));
            sprintf(payload, "%d ** %d\n", i, j);

            // send the string to the child
            write(in_fd[1], payload, strlen(payload));

            // read the result python prints to stdout
            char res[1024];
            read(out_fd[0], res, sizeof(res));

            // trim anything past \n in res
            char* newline = strchr(res, '\n');
            if (newline != NULL)
                *newline = '\0';

            // free the payload
            free(payload);

            printf("(%d) ** (%d) = %s\n", i, j, res);
        }

        // close the pipes
        close(in_fd[0]);
        close(in_fd[1]);

        close(out_fd[0]);
        close(out_fd[1]);

        printf("Child exit status: 0\n");
        kill(pid, SIGTERM);
    }

    return 0;
}
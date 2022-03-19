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

    // redirect stderr to /dev/null
    // close(2);

    // execute the python interpreter
    execl(cmd, cmd, "-c", "import sys; eval(sys.stdin.readline().strip())", NULL);
    
    // get the error message
    char *err_msg = strerror(errno);
    fprintf(stderr, "failed to execute %s: %s\n", cmd, err_msg);

    return 0;
}
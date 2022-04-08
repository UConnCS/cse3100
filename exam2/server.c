#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include <arpa/inet.h>


#define DEFAULT_PORT   "3100"
#define BACKLOG 10

#define handle_error(msg) \
         do { perror(msg); exit(-1); } while (0)

int main(int argc, char *argv[]) {
    char *port = DEFAULT_PORT;
    if (argc > 1)
        port = argv[1];

    // create a server that listens on the given port
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) handle_error("socket");

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *server_info;
    int status = getaddrinfo(NULL, port, &hints, &server_info);
    if (status != 0) handle_error("getaddrinfo");

    status = bind(server_fd, server_info->ai_addr, server_info->ai_addrlen);
    if (status == -1) handle_error("bind");

    status = listen(server_fd, BACKLOG);
    if (status == -1) handle_error("listen");

    // accept connections with a child process
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    while ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len))) {
        if (client_fd == -1) handle_error("accept");

        pid_t pid = fork();
        if (pid == 0) {
            char buf[1024];
            int bytes_read;

            while ((bytes_read = read(client_fd, buf, 1024)) > 0) {
                buf[bytes_read] = '\0';

                char *lines[80];
                int num_lines = 1;

                // get first line
                char *line = strtok(buf, "\n");
                int size = atoi(line);

                // get remaining lines
                while (line != NULL) {
                    lines[num_lines++] = line;
                    line = strtok(NULL, "\n");
                }

                // reassemble the array of lines
                char *new_buf = malloc(sizeof(char) * size);
                for (int i = 2; i < num_lines; i++) {
                    strcat(new_buf, lines[i]);
                    strcat(new_buf, "\n");
                }

                // create a child, pipe, and send the data
                pid_t child_pid = fork();
                int fd[2];
                pipe(fd);
                if (child_pid == 0) {
                    dup2(fd[0], 0);
                    dup2(client_fd, 1);
                    execl("md5sum", "md5sum", NULL);
                } else {
                    write(fd[0], new_buf, size);
                }
            }
        }
    }

    exit(0);
}

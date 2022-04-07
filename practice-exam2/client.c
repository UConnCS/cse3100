#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define DEFAULT_PORT "3100"
#define DEFAULT_HOST "cd11.engr.uconn.edu"

#define handle_error(msg) \
           do { perror(msg); exit(-1); } while (0)

int main(int argc, char **argv) {
    char* servername = DEFAULT_HOST;
    char* port = DEFAULT_PORT;

    if (argc > 1) servername = argv[1];
    if (argc > 2) port = argv[2];

    // connect to server
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *servinfo;
    int res = getaddrinfo(servername, port, &hints, &servinfo);
    if (res != 0)
        return 1;
    
    int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (sockfd == -1) handle_error("socket");

    int status = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
    if (status == -1) handle_error("connect");

    int bytes;

    // put stdin into char*
    char* input = malloc(sizeof(char) * 1024);
    fgets(input, 1024, stdin);

    // send to socket
    write(sockfd, input, strlen(input));

    // read from socket
    char* buffer = malloc(sizeof(char) * 1024);
    while (1) {
        bytes = read(sockfd, buffer, 1024);
        if (bytes == 0)
            break;
        printf("%s", buffer);
    }

    free(input);
    free(buffer);
    freeaddrinfo(servinfo);

    // close socket
    close(sockfd);
    exit(0);
}
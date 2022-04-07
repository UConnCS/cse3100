#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define DEFAULT_START_PORT 1
#define DEFAULT_END_PORT   100
#define DEFAULT_HOST       "localhost"

#define handle_error(msg) \
           do { perror(msg); exit(-1); } while (0)

// Declare any additional macros and functions here
// TODO


int main(int argc, char* argv[])
{
    int start = DEFAULT_START_PORT;
    int end   = DEFAULT_END_PORT;
    char* servername = DEFAULT_HOST;

    // args: servername argv[1], start argv[2], end argv[3]
    if (argc > 1) servername = argv[1];
    if (argc > 2) start = atoi(argv[2]);
    if (argc > 3) end = atoi(argv[3]);

    char request[] = "GET / HTTP/1.1\r\nHost: %s\r\n\r\n";

    // scan ports from start to end
    for (int port = start; port <= end; port++) {
        // create socket
        int sockfd = socket(PF_INET, SOCK_STREAM, 0);

        // connect to server
        struct sockaddr_in srv;
        struct hostent* server = gethostbyname(servername);
        srv.sin_family = AF_INET;
        srv.sin_port = htons(port);

        memcpy(&srv.sin_addr.s_addr, server->h_addr_list[0], server->h_length);

        int status = connect(sockfd, (struct sockaddr*) &srv, sizeof(srv));
        if (status == -1) {
            close(sockfd);
            continue;
        }

        // send request
        status = send(sockfd, request, strlen(request), 0);
        if (status == -1) {
            close(sockfd);
            continue;
        }

        // read response
        char response[4];
        // configure timeout
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 100000;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));
        ssize_t n = recv(sockfd, response, strlen(response), 0);
        if (n < 0) {
            printf("found active non-HTTP port: %d\n", port);
            close(sockfd);
            continue;
        }

        // if response starts with "HTTP" set flag
        int http = strncmp(response, "HTTP", 4) == 0;
        printf("found active %s port: %d\n", http ? "HTTP" : "non-HTTP", port);

        // close socket
        close(sockfd);
    }

    exit(0);
}
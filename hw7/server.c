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

#define DEFAULT_PORT "3100"
#define BACKLOG 10
#define BUF_LEN 80
#define MAX_REPLY_LEN 1000

#define handle_error(msg) \
           do { perror(msg); exit(-1); } while(0)

double stddev(double mean, double count, double values[]) {
    double sigma = 0.0;
    for (int i = 0; i < count; i++) {
        double delta = values[i] - mean;
        sigma += delta * delta;
    }

    sigma = sqrt(sigma / (count - 1));
    return sigma;
}
    
int main(int argc, char* argv[]) {
    char* port = DEFAULT_PORT;
    if (argc > 1) port = argv[1];

    // create a server that listens on the given port
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) handle_error("socket");

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* server_info;
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

    while ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len))) {
        if (client_fd == -1) handle_error("accept");

        pid_t pid = fork();
        if (pid == 0) {
            int count = 0;
            int idx = 0;
            double sum = 0.0;
            double values[1000];
            
            char buf[1024];
            char welcome[MAX_REPLY_LEN];

            int bytes_read;
            sprintf(welcome, "Stats server ready\n");
            write(client_fd, welcome, strlen(welcome));

            while ((bytes_read = read(client_fd, buf, 1024)) > 0) {
                buf[bytes_read] = '\0';

                if (strlen(buf) == 1 && (buf[0] == '\n' || buf[0] == '\r')) {
                    shutdown(client_fd, SHUT_RDWR);
                    break;
                }

                // get an array of all the lines of the string
                char* lines[BUF_LEN];
                int num_lines = 0;
                char* line = strtok(buf, "\n");
                while (line != NULL) {
                    char* start = line;
                    while (*start == ' ') start++;
                    char* end = start + strlen(start) - 1;
                    while (*end == ' ') end--;
                    *(end + 1) = '\0';
                    lines[num_lines] = start;
                    num_lines++;
                    line = strtok(NULL, "\n");
                }

                for (int i = 0; i < num_lines; i++) {
                    char* line = lines[i];

                    // line too long
                    if (strlen(line) >= 80) {
                        char error[MAX_REPLY_LEN];
                        sprintf(error, "Error: long line\n");
                        write(client_fd, error, strlen(error));
                        memset(buf, 0, strlen(buf));
                        continue;
                    }

                    // return the number of numbers sent
                    if (strcmp(line, "count") == 0) {
                        char count_str[BUF_LEN];
                        sprintf(count_str, "%d\n", count);
                        write(client_fd, count_str, strlen(count_str));
                        continue;
                    }

                    // return the mean of all numbers
                    if (strcmp(line, "mean") == 0) {
                        float mean = sum / count;
                        char mean_str[BUF_LEN];
                        sprintf(mean_str, "%.2f\n", mean);
                        send(client_fd, mean_str, strlen(mean_str), 0);
                        continue;
                    }

                    // return the standard deviation of all numbers
                    if (strcmp(line, "stddev") == 0) {
                        double mean = sum / count;
                        double sigma = stddev(mean, count, values);
                        
                        char stddev_str[BUF_LEN];
                        sprintf(stddev_str, "%.2f\n", sigma);
                        send(client_fd, stddev_str, strlen(stddev_str), 0);
                        continue;
                    }

                    // return the sum of all numbers
                    if (strcmp(line, "sum") == 0) {
                        char sum_str[BUF_LEN];
                        sprintf(sum_str, "%.2f\n", sum);
                        send(client_fd, sum_str, strlen(sum_str), 0);
                        continue;
                    }

                    // send collected stats & exit
                    if (strcmp(buf, "exit") == 0) {
                        double mean = sum / count;
                        double sigma = stddev(mean, count, values);
                        
                        char reply[MAX_REPLY_LEN];
                        sprintf(reply, "EXIT STATS: count %d mean %.2f stddev %.2f\n", count, sum / count, sigma);
                        write(client_fd, reply, strlen(reply));
                        shutdown(client_fd, SHUT_RDWR);
                        break;
                    }

                    // add a number to the stats
                    float f;
                    if (sscanf(buf, "%f", &f) == 1) {
                        count++;
                        sum += f;
                        values[idx] = f;
                        idx++;
                        continue;
                    }

                    // handle input command message
                    char reply[MAX_REPLY_LEN];
                    sprintf(reply, "Error: unrecognized command\n");
                    write(client_fd, reply, strlen(reply));
                }
            }
        }
    }
}
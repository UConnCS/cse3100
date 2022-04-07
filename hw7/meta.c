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
#include <sys/select.h>
#include <assert.h>
#include <arpa/inet.h>

#define DEFAULT_PORT        "3100"
#define DEFAULT_META_PORT   "3101"
#define BACKLOG 10
#define BUF_LEN 80
#define MAX_REPLY_LEN 1000

#define handle_error(msg) \
           do { perror(msg); exit(-1); } while (0)

int bind_fd(int port) {
    int sid = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    int status = bind(sid, (struct sockaddr*)&addr, sizeof(addr));
    if (status == -1) handle_error("bind");

    status = listen(sid, BACKLOG);
    if (status == -1) handle_error("listen");

    return sid;
}

int port_for_fd(int fd, int server, int meta) {
    if (fd == 3) return server;
    if (fd == 4) return meta;
    if (fd == 5) return 1; 
    return -1;
}

int accept_multi(int fds[], int count, struct sockaddr* addr, socklen_t addrlen, int* port, int server_port, int meta_port) {
    fd_set readfds;
    FD_ZERO(&readfds);
    for (unsigned int i = 0; i < count; i++) {
        FD_SET(fds[i], &readfds);
    }
    
    int status = select(6, &readfds, NULL, NULL, NULL);
    if (status == -1) handle_error("select");
    for (int i = 0; i < count; i++) {
        if (FD_ISSET(fds[i], &readfds)) {
            int used_fd = fds[i];
            *port = port_for_fd(used_fd, server_port, meta_port);
            return accept(fds[i], addr, &addrlen);
        }
    }
    
    return -1;
}

double stddev(double mean, double count, double values[]) {
    if (count == 0)
        return -NAN;

    double sigma = 0.0;
    for (int i = 0; i < count; i++) {
        double delta = values[i] - mean;
        sigma += delta * delta;
    }

    sigma = sqrt(sigma / (count - 1));
    return sigma;
}

int main(int argc, char* argv[]) {
    char* port      = DEFAULT_PORT;
    char* meta_port = DEFAULT_META_PORT;

    if (argc > 1) port = argv[1];
    if (argc > 2) meta_port = argv[2];

    int server_fd = bind_fd(atoi(port));
    int meta_fd = bind_fd(atoi(meta_port));
    int out_fd[2];

    pipe(out_fd);

    int mean_count = 0;
    double mean_on_exit[1000];

    while (1) {
        int used_port;
        struct sockaddr* addr = NULL;
        int client_fd = accept_multi((int[]) {server_fd, meta_fd, out_fd[0]}, 3, addr, 0, &used_port, atoi(port), atoi(meta_port));

        // stats server
        if (used_port == atoi(port)) {
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
                close(out_fd[0]);

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

                            // send the mean
                            char meta[BUF_LEN];
                            sprintf(meta, "%.2f", mean);
                            write(out_fd[1], meta, strlen(meta));

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

        if (used_port == 1) {
            char buf[BUF_LEN];
            ssize_t bytes_read = read(out_fd[0], buf, BUF_LEN);
            if (bytes_read == -1) handle_error("read");
            buf[bytes_read] = '\0';
            
            float f;
            if (sscanf(buf, "%f", &f) == 1) {
                mean_on_exit[mean_count] = f;
                mean_count += 1;
            }
        }

        // meta server
        if (used_port == atoi(meta_port)) {
            char reply[MAX_REPLY_LEN];
            double means = 0.0;
            for (int i = 0; i < mean_count; i++) {
                means += mean_on_exit[i];
            }

            means /= mean_count;
            sprintf(reply, "means_count %d mean_of_means %.2f stddev_of_means %.2f\n", mean_count, means, stddev(means, mean_count, mean_on_exit));
            write(client_fd, reply, strlen(reply));
            close(client_fd);
            continue;
        }
    }

    exit(0);
}
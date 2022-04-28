#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define DEFAULT_FILE_NAME    "start0.txt"
#define DEFAULT_CYCLES       100
#define DEFAULT_PRINT_FLAG   1
#define DEFAULT_NUM_THREADS  2

#define REFRESH_MILLISEC 1000/30
#define cycle(x) x % 2

pthread_barrier_t barrier;

void msleep(int tms) {
    struct timeval tv;
    tv.tv_sec  = tms / 1000;
    tv.tv_usec = (tms % 1000) * 1000;
    select (0, NULL, NULL, NULL, &tv);
}

typedef struct {
    int row;
    int col;
    char** src;
} Board;

typedef struct thread_result {
    Board* a;
    Board* b;
    int cycles;
    int start;
    int end;
} thread_result;

Board* makeBoard(int r, int c) {
    Board* p = malloc(sizeof(Board));
    p->row = r;
    p->col = c;
    p->src = malloc(sizeof(char*)*r);
    for(int i=0; i<r; i++)
        p->src[i] = malloc(sizeof(char)*c);
    return p;
}

void freeBoard(Board* b) {
    for (int i = 0; i < b->row; i++)
        free(b->src[i]);

    free(b->src);
    free(b);
}

Board* readBoard(char* fName) {
    int row, col;
    FILE* src = fopen(fName,"r");
    fscanf(src, "%d %d\n", &row, &col);
    Board* rv = makeBoard(row, col);

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            char ch = fgetc(src);
            rv->src[i][j] = ch == '*';
        }

        char skip = fgetc(src);
        while (skip != '\n') skip = fgetc(src);
    }

    fclose(src);
    return rv;
}

void saveBoard(Board* b, FILE* fd) {
    for (int i = 0; i < b->row; i++) {
        fprintf(fd,"|");
        for (int j = 0; j < b->col; j++)
            fprintf(fd,"%c",b->src[i][j] ? '*' : ' ');
        fprintf(fd,"|\n");
    }
}

void printBoard(Board* b) {
    printf("\033\143");  
    saveBoard(b, stdout);
}

int liveNeighbors(int i, int j, Board* b) {
    const int pc = (j-1) < 0 ? b->col-1 : j - 1;
    const int nc = (j + 1) % b->col;
    const int pr = (i-1) < 0 ? b->row-1 : i - 1;
    const int nr = (i + 1) % b->row;
    
    int xd[8] = {pc , j , nc, pc, nc, pc , j , nc };
    int yd[8] = {pr , pr, pr, i , i , nr , nr ,nr };
    int ttl = 0;
    for (int k = 0; k < 8; k++)
        ttl += b->src[yd[k]][xd[k]];

    return ttl;
}

void evolveBoard(Board* in, Board* out, int r1, int r2) {
    static int rule[2][9] = {
        { 0, 0, 0, 1, 0, 0, 1, 0, 0 },
        { 0, 0, 1, 1, 0, 0, 0, 0, 0 }
    };

    for (int i = r1; i < r2; i++) {
        for (int j = 0; j < in->col; j++) {
            int lin = liveNeighbors(i, j, in);
            int con  = in->src[i][j];
            out->src[i][j] = rule[con][lin];
        }
    }
}

void* evolveThread(void* arg) {
    thread_result* data = (thread_result*)arg;
    Board* b1 = data->a;
    Board* b2 = data->b;
    int start = data->start;
    int end = data->end;
    int cycles = data->cycles;

    Board* boards[2] = { b1, b2 };

    for (int g = 0; g < cycles; g++) {
        evolveBoard(boards[cycle(g)], boards[1 - cycle(g)], start, end);
        pthread_barrier_wait(&barrier);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    char* file_name = (argc < 2 ? DEFAULT_FILE_NAME : argv[1]);
    int cycles = (argc < 3 ? DEFAULT_CYCLES : atoi(argv[2]));
    int printFlag = (argc < 4 ? DEFAULT_PRINT_FLAG : atoi(argv[3]));
    int num_threads = (argc < 5 ? DEFAULT_NUM_THREADS : atoi(argv[4]));

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <start_file> <cycles> <print_flag> <num_threads>\n", argv[0]);
        exit(1);
    }

    thread_result result[num_threads];  
    pthread_t threads[num_threads];  
    
    Board* a = readBoard(file_name);
    Board* b = makeBoard(a->row, a->col);
    Board* boards[2] = {a, b};
    
    pthread_barrier_init(&barrier, NULL, num_threads);
        
    for (int i = 0; i < num_threads; i++) {
        int start = (i * a->row) / num_threads;                                 
        int end = i == num_threads - 1
            ? a->row
            : ((i + 1) * a->row) / num_threads;
        
        thread_result data;
        data.a = a;
        data.b = b;
        data.cycles = cycles;
        data.start = start;
        data.end = end;

        result[i] = data;
        pthread_create(&threads[i], NULL, evolveThread, (void *)&(result[i]));
    }

    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    for (int k = 0; k < cycles; k++) {
        if (printFlag) {
            printBoard(boards[k%2]);
            msleep(REFRESH_MILLISEC);
        }
    }

    if (printFlag) printBoard(boards[1 - cycle(cycles)]);

    FILE* final = fopen("final_threads.txt","w");
    saveBoard(boards[1 - cycle(cycles)], final);
    fclose(final);
    
    pthread_barrier_destroy(&barrier);
    freeBoard(a);
    freeBoard(b);
    return 0;
}
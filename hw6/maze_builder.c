#define _POSIX_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <signal.h>
#include "maze.h"

pid_t generate_cell(pid_t n, pid_t s, pid_t e, pid_t w, char* treasure_amount) {
    pid_t pid = fork();
    
    // child process
    if (pid == 0) {
        // redirect stdin to the pipe
        if (n != -1) {
            close(0);
            dup2(INPIPE_FD(NORTH), 0);
        }

        // redirect stdout to the pipe
        if (s != -1) {
            close(1);
            dup2(OUTPIPE_FD(SOUTH), 1);
        }

        // redirect stderr to the pipe
        if (e != -1) {
            close(2);
            dup2(OUTPIPE_FD(EAST), 2);
        }

        // redirect stderr to the pipe
        if (w != -1) {
            close(3);
            dup2(OUTPIPE_FD(WEST), 3);
        }

        execl("./maze_cell", "maze_cell");
        fprintf(stderr, "failed to execute maze_cell\n");
        exit(1);
    }
    
    // return child pid
    return pid;
}

/*
 * Read the info on each maze cell from "maze_file", complete the setup of
 * the pipes used by the parent process to send and receive data to/from
 * the standard input/output of cell "start_cell", setup pipes for communication
 * between connected cells, start a process executing the included "maze_cell"
 * program for each maze cell, passing "treasure_amount" as command line argument
 * to the cell process corresponding to cell "treasure_cell", and save the process
 * IDs of the maze_cell processes in the "pid" array
 */
int build_maze( FILE* maze_file,
                char* cell_executable,
                int num_cells,
                int start_cell,
                int treasure_cell,
                char* treasure_amount,
                int to_start[2],
                int from_start[2],
                pid_t* pid) {
   for (int i = 0; i < num_cells; i++) {
        // read the info on the maze cell
        int _, n, s, e, w;
        fscanf(maze_file, "%d %d %d %d %d\n", &_, &n, &s, &e, &w);
    
        // start a child process
        pid[i] = generate_cell(n, s, e, w, treasure_amount);
        if(pid[i] == -1) {
             fprintf(stderr, "failed to start child process\n");
             return -1;
        }
    
        // close unused pipe ends
        close(to_start[1]);
        close(from_start[0]);
    
        // redirect stdin and stdout to the pipe
        dup2(to_start[0], 0);
        dup2(from_start[1], 1);
    
        // close unused pipe ends
        close(to_start[0]);
        close(from_start[1]);
        return -1;
   }

   return 0;
}

int main(int argc, char* argv[])
{
   // reserve the FDs used for communication between connected cells
   // before they get used for other purposes
   INPIPE_FD_RESERVE();
   OUTPIPE_FD_RESERVE();

   // open maze file
   FILE* maze_file;
   if(argc == 1) {
      fprintf(stderr, "Usage: %s <maze_file> [<cell_executable>]\n", argv[0]);
      return -1;
   }

   maze_file = fopen(argv[1], "r");
   if(maze_file == NULL) {
      fprintf(stderr, "could not open %s\n", argv[1]);
      return -1;
   }

   char* cell_executable = "./maze_cell";
   if(argc > 2)
      cell_executable = argv[2];

   // read info from first three lines of the maze file
   int num_cells;
   fscanf(maze_file, "size: %d\n", &num_cells);

   int start_cell;
   fscanf(maze_file, "start: %d\n", &start_cell);

   int  treasure_cell;
   char treasure_amount[20]; // enough to hold a long
   fscanf(maze_file, "treasure: %d %s\n", &treasure_cell, treasure_amount);

   pid_t pid[num_cells];  // array to hold process ids for cells

   // create pipes for communication with start cell
   int to_start[2];
   int from_start[2];
   if( pipe(to_start)!=0 || pipe(from_start)!=0 ) {
      fprintf(stderr, "failed to create pipes to/from start cell\n");
         return -1;
   }

   // setup pipes and start a child process for each maze cell
   int ret = build_maze( maze_file, cell_executable, num_cells, start_cell,
                         treasure_cell, treasure_amount, to_start, from_start, pid);
   if( ret != 0 ) {
      fprintf(stderr, "failed to build the maze\n");
      return -1;
   }


   // Send lines from stdin to the start cell using the to_start pipe,
   // get back replies using the from_start pipe, and print the replies to stdout
   char buffer[MAX_PATH_LEN+21];  // extra space for treasure amount and a newline

   while(1) {

      // TODO: Update the code of the loop to perform multiplexed input and handle invalid paths

      // get input from stdin
      if( fgets(buffer, sizeof(buffer), stdin) == NULL)
         break;   // stdin has ended, exit loop and finish execution

      write(to_start[1], buffer, strlen(buffer) );  // write only bytes we got from fgets

      // read bytes from from_start
      ssize_t bytes_read = read(from_start[0], buffer, sizeof(buffer)-1 );

      // add NUL terminator for printf
      buffer[bytes_read] = '\0';
      printf("%s", buffer);
      fflush(stdout);
   }

   // done with the maze
   // close remaining open files and pipe ends and terminate all cell processes
   fclose(maze_file);
   close(to_start[1]);
   close(from_start[0]);

   for(int i=0; i<num_cells; i++)
      kill(pid[i], SIGINT);

   return 0;
}

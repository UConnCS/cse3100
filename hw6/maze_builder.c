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

/*
 * Read the info on each maze cell from "maze_file", complete the setup of
 * the pipes used by the parent process to send and receive data to/from
 * the standard input/output of cell "start_cell", setup pipes for communication
 * between connected cells, start a process executing the included "maze_cell"
 * program for each maze cell, passing "treasure_amount" as command line argument
 * to the cell process corresponding to cell "treasure_cell", and save the process
 * IDs of the maze_cell processes in the "pid" array
 * 
 * Cells are numbered sequentially in row-by-row order, starting from 0.  After the first 3 lines, which give the number of cells and info on the start and treasure cells, the maze file includes one line per cell with the number of the cell itself followed, in order, by the numbers of its connected neighbors in the N/S/E/W directions (or -1 if the respective connections do not exist). 
 * After reading the first three lines of the maze file given as command line argumemnt and some additional initializations, the provided main() function of maze_builder.c creates two pipes to be used for communication with the starting cell, then calls the function build_maze() to setup the pipes necessary for cell-to-cell communication and start a child process for each maze cell.  If build_maze() returns successfuly, the main function enters a loop in which it alternates between taking input from the user on stdin, sending the user input to a pipe connected by build_maze() to the stdin of the start cell, then waiting for an answer from the start cell via a second pipe connected by build_maze() to the stdout of the start cell.
 * Your main task is to implement build_maze().  The maze_cell program is written to use the following file descriptors for incoming/outgoing pipes from/to each of the four cardinal directions: 
 * 
 * North (in) = 10
 * North (out) = 20
 * 
 * South (in) = 11
 * South (out) = 21
 * 
 * East (in) = 12
 * East (out) = 22
 * 
 * West (in) = 13
 * West (out) = 23g
 * 
 * The file descriptors for direction d are given by the INPIPE_FD(d) and OUTPIPE_FD(d) macros defined in maze.h, which you are encouraged to use.  Thus, after creating the pipes necessary for cell-to-cell communication by calling pipe() in the parent process, and before upgrading forked children to execute the maze_cell program, you will need to use dup2() to duplicte the file descriptors for the pipe ends to these locations in the file descritor table.  Of course, you should also close all unused pipe ends. 
 * Your second task is to improve the while loop in the provided main() function of build_maze.  As written, the program only works when the user types valid paths.  If an invalid path is entered by the user, the start cell never sends back an answer to the main process. Since the read operation used in the while loop is blocking, the parent is stuck waiting for this answer even when the user enters additional paths on the standard input.  You can resolve this issue by using select, similarly to the way it is used to multiplex the input in the provided maze_cell.c progrm.
 * As usual, you should develop the code on your code-server IDE and test it using the provided maze files.  Once you are satisfied with the results obtained in the IDE submit the entire maze_builder.c on moodle.  
 * 
 * You are strongly encouraged to solve the proble in a stepwise manner.  First, get maze_builder to start a 1x1 maze.  This will only require properly setting up the pipes for comunication between the parent process and the start cell.  Then, get the program to work on 1x2 or 2x1 mazes, which will also need two unidirectional cell-to-cell pipes.  Once you got the steps worked out for the small mazes it will be easier to implement the code for arbitrary rectangular mazes.
 * 
 * Running the program:
 * (echo "NS"; sleep 1) | ./maze_builder maze_1x1.txt /opt/cse3100/maze_cell -> NS999999
 * (echo "SN"; sleep 1) | ./maze_builder maze_1x2.txt /opt/cse3100/maze_cell -> .999999999
 * (echo "EW"; sleep 1) | ./maze_builder maze_2x1.txt /opt/cse3100/maze_cell -> .999999999
 * (echo "NESNWS"; sleep 1) | ./maze_builder maze_2x2.txt /opt/cse3100/maze_cell -> ...7811287324810354840..
 * (echo "EESNW"; sleep 1; echo "EESNWW"; sleep 1) | ./maze_builder maze_2x3.txt /opt/cse3100/maze_cell -> ...2407355477030534441..
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
    int i, n, s, e, w;
    fscanf(maze_file, "%d %d %d %d %d", &i, &n, &s, &e, &w);

    for (int i = 0; i < num_cells; i++) {
        int to_cell[2];
        int from_cell[2];
        pipe(to_cell);
        pipe(from_cell);

        fscanf(maze_file, "%d %d %d %d %d", &i, &n, &s, &e, &w);

        pid[i] = fork();
        if (pid[i] == 0) {
            if (i == start_cell) {
                dup2(to_start[1], OUTPIPE_FD(NORTH));
                dup2(from_start[0], INPIPE_FD(NORTH));
            }

            if (n != -1) {
                dup2(to_cell[1], OUTPIPE_FD(NORTH));
                dup2(from_cell[0], INPIPE_FD(NORTH));
                close(to_cell[0]);
            }

            if (s != -1) {
                dup2(to_cell[1], OUTPIPE_FD(SOUTH));
                dup2(from_cell[0], INPIPE_FD(SOUTH));
                close(to_cell[0]);
            }

            if (e != -1) {
                dup2(to_cell[1], OUTPIPE_FD(EAST));
                dup2(from_cell[0], INPIPE_FD(EAST));
                close(to_cell[0]);
            }

            if (w != -1) {
                dup2(to_cell[1], OUTPIPE_FD(WEST));
                dup2(from_cell[0], INPIPE_FD(WEST));
                close(to_cell[0]);
            }

            execl(cell_executable, cell_executable, treasure_cell == i ? treasure_amount : NULL, NULL);
            fprintf(stderr, "Failed to execute %s\n", cell_executable);
        }

        // dup2(to_cell[1], OUTPIPE_FD(NORTH));
        // dup2(from_cell[0], INPIPE_FD(NORTH));
        // dup2(to_cell[1], OUTPIPE_FD(SOUTH));
        // dup2(from_cell[0], INPIPE_FD(SOUTH));
        // dup2(to_cell[1], OUTPIPE_FD(EAST));
        // dup2(from_cell[0], INPIPE_FD(EAST));
        // dup2(to_cell[1], OUTPIPE_FD(WEST));
        // dup2(from_cell[0], INPIPE_FD(WEST));
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
      fd_set readfds;
      FD_ZERO(&readfds);
      FD_SET(0, &readfds);
      FD_SET(from_start[0], &readfds);

      int fd_max = from_start[0];
      if (fd_max < 0)
         fd_max = 0;

        int ret = select(fd_max+1, &readfds, NULL, NULL, NULL);
        if (ret < 0) {
            perror("select");
            return -1;
        }

        if (FD_ISSET(0, &readfds)) {
             // read a line from stdin
             char* line = fgets(buffer, MAX_PATH_LEN+21, stdin);
             if (line == NULL) {
                // EOF on stdin
                break;
             }
    
             // send the line to the start cell
             write(to_start[1], line, strlen(line));
        }

        if (FD_ISSET(from_start[0], &readfds)) {
             // read from start cell
             int n = read(from_start[0], buffer, MAX_PATH_LEN+21);
             if (n > 0) {
                // write to stdout
                write(1, buffer, n);
             } else {
                // EOF or error
                break;
             }
        }
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

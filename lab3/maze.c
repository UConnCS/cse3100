#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum Item {
    NOTHING,
    SPELLBOOK,
    POTION,
    WAND
} Item;

typedef struct MazeCell {
   Item whatsHere; // Which item, if any, is present.
   struct MazeCell* N;
   struct MazeCell* S;
   struct MazeCell* E;
   struct MazeCell* W;
} MazeCell;

// generate a max of n cells
MazeCell* generateMaze(int n) {
    // generate random maze with random directions
    MazeCell* maze = malloc(sizeof(MazeCell));
    MazeCell* current = maze;
    for (int i = 0; i < n; i++) {
        // generate a random direction
        int direction = rand() % 4;
        switch (direction) {
            case 0:
                current->N = malloc(sizeof(MazeCell));
                current->N->S = current;
                current = current->N;
                break;
            case 1:
                current->S = malloc(sizeof(MazeCell));
                current->S->N = current;
                current = current->S;
                break;
            case 2:
                current->E = malloc(sizeof(MazeCell));
                current->E->W = current;
                current = current->E;
                break;
            case 3:
                current->W = malloc(sizeof(MazeCell));
                current->W->E = current;
                current = current->W;
                break;
        }

        printf("[%d] %p\n", i, current);
    }
    return maze;
}


void _free_maze(MazeCell* maze, char prev) {
    // given a MazeCell and the direction used to get to it, free it and subsequent cells
    if (maze == NULL) {
        return;
    }
    if (prev == 'N') {
        _free_maze(maze->N, prev);
    } else if (prev == 'S') {
        _free_maze(maze->S, prev);
    } else if (prev == 'E') {
        _free_maze(maze->E, prev);
    } else if (prev == 'W') {
        _free_maze(maze->W, prev);
    }
    printf("Freeing %p\n", maze);
    free(maze);
}

void free_maze(MazeCell* maze) {
    _free_maze(maze, 'W');   
}

int main() {
    MazeCell* maze = generateMaze(16);
    free_maze(maze);
    return 0;
}
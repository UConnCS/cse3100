# define more variables so it is easier to make changes
CC=cc
CFLAGS=-g -Wall -Werror -std=c99 -DDEBUG
TARGETS=maze_cell maze_builder

all: $(TARGETS)

#Static Pattern Rules
$(TARGETS): % : %.c maze.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o *~ $(TARGETS)

build:
	make clean
	make all
	(echo "NS"; sleep 1) | ./maze_builder maze_1x1.txt ./maze_cell
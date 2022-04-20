CC=cc
CFLAGS= -g -std=c99 -Wall -Werror -pthread
#LFLAGS= -lm

TARGET1=dotprod
OBJECTS1=dotprod.o dotprod_main.o unixtimer.o

all : $(TARGET1)

$(TARGET1): $(OBJECTS1)
	$(CC) $(CFLAGS) -o $(TARGET1) $(OBJECTS1)

clean:
	rm -f $(TARGET1) $(OBJECTS1)

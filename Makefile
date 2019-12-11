CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -g

SRC=$(wildcard *.c)
PROJ=ifj19

all: $(SRC)
	$(CC) $(CFLAGS) $^ -o $(PROJ) -lm

clean:
	rm -f $(PROJ)

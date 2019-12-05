CC=gcc
CFLAGS=-std=c99 -Wall -Wextra

SRC=$(wildcard *.c)
PROJ=ifj19

all: $(SRC)
	$(CC) $(CFLAGS) $^ -o $(PROJ)

clean:
	rm $(PROJ)

CC=gcc
CFLAGS=-Wall

SRC=$(wildcard *.c)
PROJ=ifj19

all: $(SRC)
	$(CC) $@ $^ $(CFLAGS) -o $(PROJ)
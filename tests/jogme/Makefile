CC=gcc
FLAGS=-std=c99 -Wall -Wextra -Werror -g
APP=test_scanner
LIB=../../lib/
DEPENDENCIES=$(LIB)dynamic_string.* $(LIB)errors.* $(LIB)indent_stack.* $(LIB)scanner.* $(LIB)token.*

all: $(APP)

test_scanner: $(DEPENDENCIES) scanner_test.c
	$(CC) $(FLAGS) $^ -o test_scanner

clean:
	rm -f *.gch $(LIB)*.gch $(LIB)*.gch *.o test_scanner

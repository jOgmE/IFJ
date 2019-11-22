CC=gcc
FLAGS=-std=c99 -Wall -Wextra -Werror -g
APP=scanner
LIB=../lib

all: $(APP)

scanner: scanner.* token.* $(LIB)/dynamic_string.* $(LIB)/indent_stack.*
	$(CC) $(FLAGS) -c $^

clean:
	rm *.gch $(LIB)/*.gch *.o

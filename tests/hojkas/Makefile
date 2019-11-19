
NAME=indent_stack
TEST=stack_test
#
LIBPATH=../../lib/
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -I$(LIBPATH)

$(NAME):
	$(CC) $(CFLAGS) $(TEST).c $(LIBPATH)$(NAME).c $(LIBPATH)errors.c -o $(NAME)

clean:
	rm $(NAME)

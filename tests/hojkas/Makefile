
NAME=indent_stack
TEST=stack_test
#
LIBPATH=../../lib/
SCANPATH=../../scanner/
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -I$(LIBPATH)

$(NAME):
	$(CC) $(CFLAGS) $(TEST).c $(LIBPATH)$(NAME).c $(LIBPATH)errors.c -o $(NAME)

ac:
	$(CC) $(CFLAGS) ac_test.c $(LIBPATH)*.c $(SCANPATH)*.c -o ac

clean:
	rm $(NAME) ac

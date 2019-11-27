
NAME=indent_stack
TEST=stack_test
#
LIBPATH=../../lib/
SCANPATH=../../scanner/
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -I$(LIBPATH)

p:
	$(CC) $(CFLAGS) p_test.c $(LIBPATH)*.c $(SCANPATH)*.c -o p

$(NAME):
	$(CC) $(CFLAGS) $(TEST).c $(LIBPATH)$(NAME).c $(LIBPATH)errors.c -o $(NAME)

ac:
	$(CC) $(CFLAGS) ac_test.c $(LIBPATH)*.c $(SCANPATH)*.c -o ac

st:
	$(CC) $(CFLAGS) st-test.c $(LIBPATH)adress_code.c $(LIBPATH)dynamic_string.c $(LIBPATH)errors.c $(LIBPATH)indent_stack.c $(LIBPATH)symtable.c $(SCANPATH)*.c $(SCANPATH)*.h -o st

clean:
	rm $(NAME) ac st

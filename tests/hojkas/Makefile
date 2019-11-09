
NAME=stack
TEST=stack_test
#
STACKPATH=../../lib/
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -I$(STACKPATH)

$(NAME): $(TEST).c $(STACKPATH)$(NAME).c
	$(CC) $(CFLAGS) -o $@ $(TEST).c $(STACKPATH)$(NAME).c

clean:
	rm $(NAME)

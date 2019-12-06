LIBPATH=../../
CC=gcc
CFLAGS=-Wall -std=c99 -pedantic -lm -I$(LIBPATH)

all:
	$(CC) $(CFLAGS) test.c $(LIBPATH)adress_code.*  $(LIBPATH)dynamic_string.*  $(LIBPATH)errors.* $(LIBPATH)symtable.* $(LIBPATH)token.* $(LIBPATH)PACharStack_lib.* $(LIBPATH)precAnalysis_lib.* $(LIBPATH)PATable.* -o f_test

fake:
	$(CC) $(CFLAGS) test.c $(LIBPATH)adress_code.*  $(LIBPATH)dynamic_string.*  $(LIBPATH)errors.* $(LIBPATH)symtable.* $(LIBPATH)token.* -o f_test

clean:
	rm f_test

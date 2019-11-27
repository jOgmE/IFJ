#include "PACharStack_lib.h"

void testHeader(int *i)
{
	printf("TEST #%d -- ", *i);
	(*i)++;
}

void main ()
{
	int *i; *i = 0;

	testHeader(i);
	PAStack *s;
	PAInit(&s);

	testHeader(i);
	PAPushFin(s);

	testHeader(i);
	PAYeet(s);
}

#include <assert.h>
#include "PACharStack_lib.h"
#include "../../scanner/token.h"

void testHeader(int *i)
{
	printf("TEST #%d -- ", *i);
	(*i)++;
}

void main ()
{
	printf("No way.\n");
	int i;
	i = 1;
	printf("Really?\n");

	testHeader(&i);
	PAStack *s;
	PAInit(&s);
	assert(s->top == NULL);

	testHeader(&i);
	Token *testToken = init_token();
	add_int(testToken, 3);
	PAPush(s, testToken);
	assert(s->top->c == 'i'); 


	testHeader(&i);
	PAPushFin(s);
	assert(s->top->c == '$');

	testHeader(&i);
	PAYeet(s);

	free_token(testToken);
}

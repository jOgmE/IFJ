#include <assert.h>
#include "PACharStack_lib.h"
#include "../../scanner/token.h"

void testHeader(int *i)
{
	printf("TEST #%d -- \n", *i);
	(*i)++;
}

void main ()
{
	int i;
	i = 1;

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
	s->top->c = 'E';
	assert(PATopTerm(s)->type == INT);

	PAPop(s);
	
	testHeader(&i); // 5
	PAAddBracket(s);
	printf("Bracket added\n");
	printf("BRACKET ADDED\n");

	testHeader(&i);
	printf("4header\n");
	PAPush(s, testToken);

	Token *resToken = init_token();

	testHeader(&i); // 7
	printf("Rule about to be applied.\n");
	PAApplyRule(s, resToken);
	printf("diditwork.mp3\n");

	testHeader(&i);
	PAYeet(s);

	free_token(testToken);
	free_token(resToken);
}


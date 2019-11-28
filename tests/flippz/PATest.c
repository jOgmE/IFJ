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
	PAPushFin(s);	// :$

	testHeader(&i);
	Token *testToken = init_token();
	add_int(testToken, 3);
	PAPush(s, testToken);	// :$i
	assert(s->top->c == 'i'); 


	testHeader(&i);
	PAPushFin(s);	// :$i$
	assert(s->top->c == '$');

	testHeader(&i);
	s->top->c = 'E';	// :$iE
	assert(PATopTerm(s)->type == INT);

	PAPop(s);		// :$i
	
	testHeader(&i); // 5
	PAAddBracket(s);	// :$i[
	
	testHeader(&i);

	PAPush(s, testToken);	// :$i[i

	Token *resToken = init_token();

	testHeader(&i); // 7

	PAApplyRule(s, resToken);	// :$i[i

	// from here on this has to be remade

	testHeader(&i);
	PAPop(s);
	PAApplyRule(s, resToken);	

	// up until here

	testHeader(&i);
	PAYeet(s);

	free_token(testToken);
	free_token(resToken);
}


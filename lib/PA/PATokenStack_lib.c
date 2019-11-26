// written and directed by: Filip Draber
// Date: 24. 11. 2019

#include <stdio.h>
#include <stdlib.h>
#include "symbolStack.h"
#include "token.h"

#ifndef _PA_STACK
#define _PA_STACK

struct patokenstackelement {
	e_type content;
	int isTerm;
	struct patokenstackelement *belowPtr;
}

struct patokenstack {
	PATokenStackElem *top;
}

// initializes the stack
void PAInit ( PAStack **s )
{
	*s = (PAStack *) malloc(sizeof(PAStack));

	(*s)->top = NULL;
}

// pushes a new item to the top of the stack
void PAPush ( PAStack *s, e_type content, int isTerm )
{
	sStackElem *PAStackElem = (PAStackElem *) malloc(sizeof(sStackElem));

	newElem->content = content;
	newElem->isTerm = isTerm;

	newElem->belowPtr = s->top;

	s->top = newElem;
}

// returns the Content of the nonterminal closest to the top
e_type PAClosestTerm ( PAStack *s )
{
	PAStackElem *elem = s->top;

	while ( elem != NULL ) {
		
		if ( elem->isTerm ) {
			return elem->content;
		} else {
			elem = elem->belowPtr;
		}

	}


	// DO ERROR HERE
	return 0;
}

// podiva se na vrchol zasobniku a hleda "<y", kde y je neterminal
int PACheckTopTwo ( PAStack *s )
{

}

void PAPop ( PAStack *s )
{
	if ( s->top == NULL ) return;

	PAStackElem *tmp = s->top;
	s->top = s->top->belowPtr;
	free(tmp);

}

void PAYeet ( PAStack *s )
{
	while ( s->top != NULL ) {
		PAPop(s);
	}

	free(s);
}

#endif

// ALL IS DONE

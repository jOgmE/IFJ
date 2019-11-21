// disclaimer : the name of this file and the stack itself has nothing to do with the symtable

#include <stdio.h>
#include <stdlib.h>
#include "symbolStack.h"

// initializes the stack
void SSInit( SStack **s )
{
	*s = (SStack *) malloc(sizeof(SStack));

	(*s)->top = NULL;
}

// pushes a new item to the top of the stack
void SSPush ( SStack *s, char Content, int isTerm )
{
	sStackElem *newElem = (sStackElem *) malloc(sizeof(sStackElem));

	newElem->Content = Content;
	newElem->isTerm = isTerm;

	newElem->belowPtr = s->top;

	s->top = newElem;
}

// returns the Content of the nonterminal closest to the top
char SSClosestTerm ( SStack *s )
{
	sStackElem *elem = s->top;

	while ( elem != NULL ) {
		
		if ( elem->isTerm ) {
			return elem->Content;
		} else {
			elem = elem->belowPtr;
		}

	}


	// DO ERROR HERE
	return 0;
}

void SSPop ( SStack *s )
{
	if ( s->top == NULL ) return;

	sStackElem *tmp = s->top;
	s->top = s->top->belowPtr;
	free(tmp);

}

void SSYeet ( SStack *s )
{
	while ( s->top != NULL ) {
		SSPop(s);
	}

	free(s);
}

// ALL IS DONE

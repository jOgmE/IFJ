// written and directed by: Filip Draber
// Date: 24. 11. 2019

#include <stdio.h>
#include <stdlib.h>
#include "symbolStack.h"
#include "token.h"

#ifndef _PA_STACK
#define _PA_STACK

typedef enum { EXPR, TERM, LBR, FIN } elemType;

// polozka stacku
struct patokenstackelement {
	elemType type;
	Token content;	// TODO update other functions
	struct patokenstackelement *belowPtr;
}


// struktura stacku
struct patokenstack {
	PATokenStackElem *top;
}



// inicializace stacku
void PAInit ( PAStack **s )
{
	*s = (PAStack *) malloc(sizeof(PAStack));

	(*s)->top = NULL;
}

// push otazniku do stacku
void PAPushFin ( PAStack *s )
{
	PAStackElem *newElem = (PAStackElem *) malloc(sizeof(PAStackElem));

	newElem->type = FIN;

	newElem->belowPtr = s->top;
	s->top = newElem;
}


// push noveho terminalu do stacku
void PAPushTerm ( PAStack *s, e_type content )
{
	PAStackElem *newElem = (PAStackElem *) malloc(sizeof(PAStackElem));

	newElem->type = TERM;
	newElem->content = content;

	newElem->belowPtr = s->top;

	s->top = newElem;
}


// push lbrackety do stacku
void PAPushLBR ( PAStack *s )
{
	PAStackElem *newElem = (PAStackElem *) malloc(sizeof(PAStackElem));

	newElem->type = LBR;

	newElem->belowPtr = s->top;
	s->top = newElem;
}


// vrati hodnotu nejvyssiho terminalu na stacku
e_type PATop ( PAStack *s )
{
	PAStackElem *elem = s->top;

	while ( elem != NULL ) {
		
		if ( elem->type == TERM ) {
			return elem->content;
		} else {
			elem = elem->belowPtr;
		}

	}


	// DO ERROR HERE
	return 0;
}



// zameni nejvyssi terminal 'i' za "i<"
void PAAddBracket ( PAStack *s )
{
	PAStackElem *elem = s->top;

	if ( elem == NULL ) return;

	if ( elem->type == TERM ) {
		
		PAStackElem *newElem = (PAStackElem *) malloc(sizeof(sStackElem));

		newElem->type = LBR;

		newElem->belowPtr = s->top;
		s->top = newElem;
		
		return;	
	}

	// else

	do {

		if ( elem->belowPtr->type == TERM ) {
			
			PAStackElem *newElem = (PAStackElem *) malloc(sizeof(sStackElem));

			newElem->type = LBR;

			newElem->belowPtr = elem->belowPtr;
			elem->belowPtr = newElem;

			return;

		} else {

			elem = elem->belowPtr;
		}

	} while ( elem != NULL );

	fprintf(stderr, "No terminal to swap in PAAddBracket.\n");
}


// upravi vrchol zasobniku podle pravidel E->i, E->(E), E->E<OP>E
int PAApplyRule ( PAStack *s )
{
	PAstackElem* tempStack[4];
	int i = 0;

	PAstackElem *ptr = s->top;

	// fix pls
	while ( i < 4 ) {
	
		if ( ptr == NULL ) return 0; // oof

		if ( ptr->type == FIN ) break;

		tempStack[3-i] = s->top;
		ptr = ptr->belowPtr;
		i++;	

	}

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

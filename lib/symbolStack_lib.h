#ifndef _SSTACK_H_
#define _SSTACK_H_

// an element of the stack
typedef struct symbolStackElement {
	char Content;
	int isTerm;
	struct symbolStackElement *belowPtr;	// points at the element below this in the stack
} sStackElem;

// stack itself
typedef struct symbolStack {
	sStackElem *top;
} SStack;

void SSInit ( SStack **s );

void SSPush ( SStack *s, char Content, int isTerm );

char SSClosestTerm ( SStack *s );

void SSPop ( SStack *s );

void SSYeet ( SStack *s );

#endif //_SSTACK_H_

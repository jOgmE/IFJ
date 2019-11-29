// written and directed by: Filip Draber
// Date: 24. 11. 2019

#include "PACharStack_lib.h"

/* polozka stacku
struct pastackelement {
	char c;
	Token *content;	// TODO update other functions
	struct pastackelement *belowPtr;
};*/


/* struktura stacku
struct pastack {
	PAStackElem *top;
};*/



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

	newElem->c = '$';

	newElem->belowPtr = s->top;
	s->top = newElem;
}



// push noveho terminalu do stacku
void PAPush ( PAStack *s, Token *content )
{
	PAStackElem *newElem = (PAStackElem *) malloc(sizeof(PAStackElem));

	char c;
	switch (content->type) {
		case ID: case INT: case DEC: case STR:
			c = 'i';
			break;
		case L: case LEQ: case G: case GEQ: case EQ: case NEQ: case PLUS: case MINUS: case AST: case SL: case DSL:
			c = '+';
			break;
		case LPA:
			c = '(';
			break;
		case RPA:
			c = ')';
			break;
		default:
			c = '$';
			break;
	}

	newElem->c = c;
	newElem->content = content;

	newElem->belowPtr = s->top;

	s->top = newElem;
}



// push E do stacku (pro testovaci ucely)
void PAPushETest ( PAStack *s, Token *content)
{
	PAStackElem *newElem = (PAStackElem *) malloc(sizeof(PAStackElem));

	newElem->c = 'E';
	newElem->content = content;

	newElem->belowPtr = s->top;
	s->top = newElem;
}



// vrati hodnotu nejvyssiho terminalu na stacku
Token *PATopTerm ( PAStack *s )
{
	PAStackElem *elem = s->top;

	while ( elem != NULL ) {
		
		if ( elem->c != 'E' ) {
			return elem->content;
		} else {
			elem = elem->belowPtr;
		}

	}


	// DO ERROR HERE
	return 0;
}



// vrati Token na vrcholu zasobniku, nebo NULL
Token *PATop ( PAStack *s )
{
	if ( s->top == NULL ) return NULL;

	return s->top->content;
}



// zameni nejvyssi terminal "<term>" za "<term>["
void PAAddBracket ( PAStack *s )
{
	PAStackElem *elem = s->top;

	if ( elem == NULL ) return;

	if ( elem->c != 'E' ) {
		
		PAStackElem *newElem = (PAStackElem *) malloc(sizeof(PAStackElem));

		newElem->c = '[';


		newElem->belowPtr = s->top;
		s->top = newElem;
		
		return;	
	}

	// else

	do {

		if ( elem->belowPtr->c != 'E' ) {
			
			PAStackElem *newElem = (PAStackElem *) malloc(sizeof(PAStackElem));

			newElem->c = '[';

			newElem->belowPtr = elem->belowPtr;
			elem->belowPtr = newElem;

			return;

		} else {

			elem = elem->belowPtr;
		}

	} while ( elem != NULL );

	fprintf(stderr, "No terminal to swap in PAAddBracket.\n");
}





// provede zpetnou derivaci E->i
int PACodeRule1 ( Token *i )
{

	out = initToken();
	

}

// provede zpetnou derivaci E->E+E
int PACodeRule2 ( )
{

}

// provede zpetnou derivaci E->(E)
int PACodeRule3 ( )
{

}


// upravi vrchol zasobniku podle pravidel E->i, E->(E), E->E<OP>E
int PAApplyRule ( PAStack *s, Token *res )
{
	PAStackElem* tempStack[4];
	int i = 0;

	PAStackElem *ptr = s->top;

	// nakopiruje top ctyri znaky ze zasobniku do pole
	while ( i < 4 ) {
	
		if ( ptr == NULL ) {
			printf("Error.\n"); //TODO vnitrni error
			return 0;
		}

		tempStack[3-i] = ptr;
		printf("tempStack[%d] = %c\n", 3-i, tempStack[3-i]->c);
		
		if ( ptr->c == '$' || ptr->c == '[' ) break;

		ptr = ptr->belowPtr;
		i++;	
	}

	printf("after cycle i = %d\n", i);

	if ( i == 1 ) {
		if ( tempStack[2]->c == '[' && tempStack[3]->c == 'i' ) {
				
			printf("Derivace E->i\n");
			return 0;
			// TODO E->i

		} else {
			printf("Error.\n"); //TODO error
			return 1;
		}



	} else if ( i == 3 ) {

		if ( tempStack[0]->c == '[' ) {

			if ( tempStack[1]->c == 'E' && tempStack[2]->c == '+' && tempStack[3]->c == 'E' ) {

				printf("Derivace E->E+E\n");
				return 0;
				// TODO E->E+E

			} else if ( tempStack[1]->c == '(' && tempStack[2]->c == 'E' && tempStack[3]->c == ')' ) {

				printf("Derivace E->(E)\n");
				return 0;
				// TODO E->(E)

			} else {
				printf("Error.\n"); // TODO error
				return 1;
			}

		} else {
			printf("Error.\n"); // TODO error
			return 1;
		}
	} else {
		printf("Error.\n");
		return 1;
	}

}



// vyhodi vrchni polozku
void PAPop ( PAStack *s )
{
	if ( s->top == NULL ) return;

	PAStackElem *tmp = s->top;
	s->top = s->top->belowPtr;
	free(tmp);

}



// uvolni cely stack
void PAYeet ( PAStack *s )
{
	while ( s->top != NULL ) {
		PAPop(s);
	}

	free(s);
}


// ALL IS DONE

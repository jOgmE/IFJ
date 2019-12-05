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
	newElem->content = NULL;

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



// push E do stacku (pro pravidla)
void PAPushE ( PAStack *s, Token *content)
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



// vrati 1 pokud je na vrcholu '$', jinak 0
int PAEndAtTop ( PAStack *s )
{
	if (s->top == NULL ) return 0;

	if (s->top->c == '$' ) return 1;
	else return 0;
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
int PACodeRule1 ( PAStack *s , Token *res )
{
	Token *tmp = copy_token(s->top->content);

	switch (s->top->content->type) {

		case ID:
			add_undef_id_from_token(s->top->content);
			switch (get_id_type(s->top->content)) {
				case ST_VALUE:
				case ST_UNDEFINED:
								
					PAPop(s);	// popne 'i'
					PAPop(s);	// popne '['

					PAPushE(s, tmp);		
					res = tmp;
				
					return 0;
					break;
				case ST_LABEL:
				case ST_FUNCTION:
					// TODO sem error
				default:
					break; // projde na return 1;
				
			}
			break;
		case INT:
		case STR:
		case DEC:
			
			PAPop(s);	// popne 'i'
			PAPop(s);	// popne '['

			PAPushE(s, tmp);
			res = tmp;		

			return 0;
			break;
		default:
			break; // projde na return 1
	}
	
	free_token(tmp);
	// TODO internal error
	return 1;
}

ac_type getACOP ( e_type op )
{
	switch ( op ) {
		case L: return LESS;
		case LEQ: return LE;
		case G: return GREATER;
		case GEQ: return GE;
		case EQ: return EQUAL;
		case NEQ: return NOT_EQUAL;
		case PLUS: return ADD;
		case MINUS: return SUB;
		case AST: return MUL;
		case SL: return DIV;
		case DSL: return DIVINT;
		default: return 0;
	}
}

// provede zpetnou derivaci E->E+E
int PACodeRule2 ( PAStack *s, Token *E1, Token *op, Token *E2, Token *res )
{
	Token *result = init_token();

	appendAC(getACOP(op->type), E1, E2, copy_token(result));

	PAPop(s);
	PAPop(s);
	PAPop(s);
	PAPop(s); // popne "[E+E"

	PAPushE(s, result);
	res = result;

	return 0;	
}

// provede zpetnou derivaci E->(E)
int PACodeRule3 ( PAStack *s, Token *res )
{
	PAPop(s); // popne ')'

	Token *tmp = copy_token(s->top->content);

	switch (s->top->content->type) {

		case ID:
			add_undef_id_from_token(s->top->content);
			switch (get_id_type(s->top->content)) {
				case ST_VALUE:
				case ST_UNDEFINED:
									
					PAPop(s);	// popne 'E'
					PAPop(s);	// popne '('
					PAPop(s);	// popne '['

					PAPushE(s, tmp);		
					res = tmp;

					return 0;
					break;
				case ST_LABEL:
				case ST_FUNCTION:
					// TODO sem error
				default:
					break; // projde na return 1;
				
			}
			break;
		case INT:
		case STR:
		case DEC:
				
			PAPop(s);	// popne 'E'
			PAPop(s);	// popne '('
			PAPop(s);	// popne '['

			PAPushE(s, tmp);	
			res = tmp;	

			return 0;
			break;
		default:
			break; // projde na return 1
	}

	free_token(tmp);
	// TODO internal error
	return 1;

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
				
			printf("PA: Derivace E->i\n");
			PACodeRule1(s, res);
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
				PACodeRule2(s, tempStack[1]->content, tempStack[2]->content, tempStack[3]->content, res);
				return 0;
				// TODO E->E+E

			} else if ( tempStack[1]->c == '(' && tempStack[2]->c == 'E' && tempStack[3]->c == ')' ) {

				printf("Derivace E->(E)\n");
				PACodeRule3(s, res);
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

	if ( s->top->content != NULL ) free_token(s->top->content);
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

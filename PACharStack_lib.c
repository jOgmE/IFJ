// written and directed by: Filip Draber
// Date: 24. 11. 2019

#include "PACharStack_lib.h"

// inicializace stacku
void PAInit ( PAStack **s )
{
	*s = (PAStack *) malloc(sizeof(PAStack));
	if ( *s == NULL ) {
		kill_after_analysis = true;
		global_error_code = INTERNAL_ERROR;
		print_internal_error(INTERNAL_ERROR, ERROR, "Chyba alokace pameti.");
	}

	(*s)->top = NULL;
}



// push otazniku do stacku
void PAPushFin ( PAStack *s )
{
	PAStackElem *newElem = (PAStackElem *) malloc(sizeof(PAStackElem));
	if ( newElem == NULL ) {
		kill_after_analysis = true;
		global_error_code = INTERNAL_ERROR;
		print_internal_error(INTERNAL_ERROR, ERROR, "Chyba alokace pameti.");
	}

	newElem->c = '$';
	newElem->content = NULL;

	newElem->belowPtr = s->top;
	s->top = newElem;
}



// push noveho terminalu do stacku
void PAPush ( PAStack *s, Token *content )
{
	PAStackElem *newElem = (PAStackElem *) malloc(sizeof(PAStackElem));
	if ( newElem == NULL ) {
		kill_after_analysis = true;
		global_error_code = INTERNAL_ERROR;
		print_internal_error(INTERNAL_ERROR, ERROR, "Chyba alokace pameti.");
	}

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

	printf("Pushed %c\n", c);
}



// push E do stacku (pro pravidla)
void PAPushE ( PAStack *s, Token *content)
{
	PAStackElem *newElem = (PAStackElem *) malloc(sizeof(PAStackElem));
	if ( newElem == NULL ) {
		kill_after_analysis = true;
		global_error_code = INTERNAL_ERROR;
		print_internal_error(INTERNAL_ERROR, ERROR, "Chyba alokace pameti.");
	}

	newElem->c = 'E';
	newElem->content = content;

	newElem->belowPtr = s->top;
	s->top = newElem;
	printf("Pushed E\n");
}



// vrati polozku nejvyssiho terminalu na stacku
PAStackElem *PATopTerm ( PAStack *s )
{
	PAStackElem *elem = s->top;

	while ( elem != NULL ) {
		
		if ( elem->c != 'E' ) {
			return elem;
		} else {
			elem = elem->belowPtr;
		}

	}


	// TODO ERROR HERE
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

	if (PATopTerm(s)->c == '$' ) return 1;
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

	printf("[filip] No terminal to swap in PAAddBracket.\n");
}




// funkce PACODERULE# jsou pouze vynaty z funkce ApplyRule, ktera uz je i tak dlouha

// provede zpetnou derivaci E->i
int PACodeRule1 ( PAStack *s )
{
	Token *tmp = copy_token(s->top->content);

	switch (s->top->content->type) {

		case ID:
			add_undef_id_from_token(s->top->content, ST_VALUE);
			switch (get_id_type(s->top->content)) {
				case ST_VALUE:
				case ST_UNDEFINED:
								
					PAPop(s);	// popne 'i'
					PAPop(s);	// popne '['

					PAPushE(s, tmp);		
				
					return 0;
					break;
				case ST_LABEL:
				case ST_FUNCTION:
					kill_after_analysis = true;
					global_error_code = SEMANTIC_DEFINITION_ERROR;
					print_compile_error(SEMANTIC_DEFINITION_ERROR, ERROR, line_count, "Spatny identifikator ve vyrazu.");
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

			return 0;
			break;
		default:
			break; // projde na return 1
	}
	
	free_token(tmp);
	printf("[filip] Typ tokenu propadl switchem.\n");
	return 1;
}

// na zaklade typu operatoru vrati prislusny typ volani AC
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

void appendNumber ( cstring *str, int *i ) {
	int j = *i;
	*i = *i + 1;
	do {
		append_char(str, (char)(j % 10) + '0');
		j = j / 10;
	} while ( j > 0 );
}


// provede zpetnou derivaci E->E+E
int PACodeRule2 ( PAStack *s, Token *E1, Token *op, Token *E2, Token *res, int *tempInt )
{
	Token *result = init_token();

	// TODO give em namesV!

	cstring *tempName = init_cstring("temp_");

	appendNumber(tempName, tempInt);

	add_temp_id(result, tempName);

	//printf("owo im a cute widdwe pwint\n");

	appendAC(getACOP(op->type), E1, E2, copy_token(result));


	PAPop(s);
	PAPop(s);
	PAPop(s);
	PAPop(s); // popne "[E+E"


	PAPushE(s, result);

	changeLastRes(res);

	return 0;	
}

// provede zpetnou derivaci E->(E)
int PACodeRule3 ( PAStack *s )
{
	printf("Did we even get here\n");
	PAPop(s); // popne ')'

	Token *tmp = s->top->content;
	
	PAPop(s); // popne 'E'
	PAPop(s); // popne '('
	PAPop(s); // popne '['

	PAPushE(s, tmp); // vrati 'E'

	return 0;
}


// upravi vrchol zasobniku podle pravidel E->i, E->(E), E->E<OP>E
int PAApplyRule ( PAStack *s, Token *res, int *tempInt)
{
	PAStackElem* tempStack[4];
	int i = 0;

	PAStackElem *ptr = s->top;

	// nakopiruje top ctyri znaky ze zasobniku do pole
	while ( i < 4 ) {
	
		if ( ptr == NULL ) {
			printf("[filip] Vnitrni error aplikace pravidla.\n");
			return 0;
		}

		tempStack[3-i] = ptr;
		printf("tempStack[%d] = %c\n", 3-i, tempStack[3-i]->c);
		
		if ( ptr->c == '$' || ptr->c == '[' ) break;
		// ^ skonci pri nalezeni '[' (OK) nebo '$' (Uh oh)

		ptr = ptr->belowPtr;
		i++;	
	}

	if ( i == 1 ) {
		if ( tempStack[2]->c == '[' && tempStack[3]->c == 'i' ) {
			
			// PROVEDENI E->i
			
			PACodeRule1(s);
			return 0;
			

		} else {
			print_compile_error(SYNTAX_ANALYSIS_ERROR, ERROR, line_count, "Chyba ve vyrazu.");
			return 1;
		}



	} else if ( i == 3 ) {

		if ( tempStack[0]->c == '[' ) {

			if ( tempStack[1]->c == 'E' && tempStack[2]->c == '+' && tempStack[3]->c == 'E' ) {

				// PROVEDENI E->E+E
				PACodeRule2(s, tempStack[1]->content, tempStack[2]->content, tempStack[3]->content, res, tempInt);
				return 0;
				
			} else if ( tempStack[1]->c == '(' && tempStack[2]->c == 'E' && tempStack[3]->c == ')' ) {

				// PROVEDENI E->(E)
				PACodeRule3(s);
				return 0;

			} else {
				print_compile_error(SYNTAX_ANALYSIS_ERROR, ERROR, line_count, "Chyba ve vyrazu.");
				return 1;
			}

		} else {
			print_compile_error(SYNTAX_ANALYSIS_ERROR, ERROR, line_count, "Chyba ve vyrazu.");
			return 1;
		}
	} else {
		print_compile_error(SYNTAX_ANALYSIS_ERROR, ERROR, line_count, "Chyba ve vyrazu.");
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

// vypise charactery na stacku
void PAPrint ( PAStack *s )
{
	if (s->top == NULL ) return;

	PAStack *temp;
	PAInit(&temp);


	PAStackElem *s_ptr = s->top;

	printf("Print.\n");
	while ( s_ptr != NULL ) {
		printf("(%c)", s_ptr->c);
		if ( s_ptr->c == '$' ) { PAPushFin(temp); }
		else PAPush(temp, s_ptr->content);
		s_ptr = s_ptr->belowPtr;
	}

	printf("STACK: >");
	while ( temp->top != NULL ) {
		printf(" %c", temp->top->c);
		PAPop(temp);
	}
	printf("\n");

	PAYeet(temp);
}


// ALL IS DONE

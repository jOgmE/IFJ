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

	printf("[filip] Funkce PATopTerm nevraci hodnotu.\n");
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


// pripne to k cstringu cislo, ktero zvysi
void appendNumber ( cstring *str, int *i ) {
	int j = *i;
	*i = *i + 1;
	do {
		append_char(str, (char)(j % 10) + '0');
		j = j / 10;
	} while ( j > 0 );
}



// funkce PACODERULE# jsou pouze vynaty z funkce ApplyRule, ktera uz je i tak dlouha

// provede zpetnou derivaci E->i
int PACodeRule1 ( PAStack *s, int *tempInt)
{	
	Token *tmp = (s->top->content->type == ID) ? copy_token(s->top->content) : init_token();
	cstring *tempName = init_cstring("temp_");

	// kopie 'i'

	switch (s->top->content->type) {

		case ID:
			free_cstring(tempName);
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
					return 0;
				default:
					break; // projde na return 1;
				
			}
			break;
		case INT:
		case STR:
		case DEC:
			
			appendNumber(tempName, tempInt);
			add_temp_id(tmp, tempName);	
			
			appendAC(ASSIGN,copy_token(s->top->content),NULL,copy_token(tmp));
			
			PAPop(s);	// popne 'i'
			PAPop(s);	// popne '['

			PAPushE(s, tmp);
					

			return 0;
			break;
		default:
			break; // projde na return 1
	}
	
	free_cstring(tempName);
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

// provede zpetnou derivaci E->E+E
int PACodeRule2 ( PAStack *s, Token *E1, Token *op, Token *E2, int *tempInt )
{
	Token *result = init_token();
	//printf("PRE: E1: %s .. E2: %s\n", get_cstring_string(E1->str), get_cstring_string(E2->str));

	cstring *tempName = init_cstring("temp_");

	appendNumber(tempName, tempInt);
	//printf("tempName for result: %s\n", get_cstring_string(tempName));

	add_temp_id(result, tempName);

	//printf("MED: E1: %s .. E2: %s\n", get_cstring_string(E1->str), get_cstring_string(E2->str));
	appendAC(getACOP(op->type), copy_token(E1), copy_token(E2), copy_token(result));
	//printf("POST: E1: %s .. E2: %s\n", get_cstring_string(E1->str), get_cstring_string(E2->str));


	PAPop(s);
	PAPop(s);
	PAPop(s);
	PAPop(s); // popne "[E+E"


	PAPushE(s, result);

	//printf("Name at top: %s\n", get_cstring_string(s->top->content->str));

	return 0;	
}

// provede zpetnou derivaci E->(E)
int PACodeRule3 ( PAStack *s )
{
	PAPop(s); // popne ')'

	Token *tmp = copy_token(s->top->content);
	
	PAPop(s); // popne 'E'
	PAPop(s); // popne '('
	PAPop(s); // popne '['

	PAPushE(s, tmp); // vrati 'E'
	//printf("Name at top: %s\n", get_cstring_string(s->top->content->str));

	return 0;
}


// upravi vrchol zasobniku podle pravidel E->i, E->(E), E->E<OP>E
int PAApplyRule ( PAStack *s, int *tempInt)
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
		
		if ( ptr->c == '$' || ptr->c == '[' ) break;
		// ^ skonci pri nalezeni '[' (OK) nebo '$' (Uh oh)

		ptr = ptr->belowPtr;
		i++;	
	}

	if ( i == 1 ) {
		if ( tempStack[2]->c == '[' && tempStack[3]->c == 'i' ) {
			
			// PROVEDENI E->i
			
			PACodeRule1(s, tempInt);
			//printf("Rule one.\n");
			return 0;
			

		} else {
			print_compile_error(SYNTAX_ANALYSIS_ERROR, ERROR, line_count, "Chyba ve vyrazu.");
			return 1;
		}



	} else if ( i == 3 ) {

		if ( tempStack[0]->c == '[' ) {

			if ( tempStack[1]->c == 'E' && tempStack[2]->c == '+' && tempStack[3]->c == 'E' ) {

				// PROVEDENI E->E+E
				PACodeRule2(s, tempStack[1]->content, tempStack[2]->content, tempStack[3]->content, tempInt);
				//printf("Rule two.\n");
				return 0;
				
			} else if ( tempStack[1]->c == '(' && tempStack[2]->c == 'E' && tempStack[3]->c == ')' ) {

				// PROVEDENI E->(E)
				PACodeRule3(s);
				//printf("Rule three.\n");
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


// ALL IS DONE

#include "token.h"

#ifndef _PA_TABLE
#define _PA_TABLE

// tabulka precedencni analyzy
const char PATable[18][18] = {
	         // ID    INT   DEC   STR   L     LEQ   G     GEQ   EQ    NEQ   PLUS  MINUS AST   SL    DSL   LPA   RPA   "$"
	/* ID	*/ {' ',  ' ',  ' ',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ' ',  ']',  ']'},
	/* INT	*/ {' ',  ' ',  ' ',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ' ',  ']',  ']'},
	/* DEC	*/ {' ',  ' ',  ' ',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ' ',  ' ',  ']',  ']'},
	/* STR 	*/ {' ',  ' ',  ' ',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ' ',  ' ',  ' ',  ' ',  ' ',  ']',  ']'},
	/* L	*/ {'[',  '[',  '[',  '[',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  '[',  '[',  '[',  '[',  '[',  '[',  ']',  ']'},
	/* LEQ	*/ {'[',  '[',  '[',  '[',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  '[',  '[',  '[',  '[',  '[',  '[',  ']',  ']'},
	/* G	*/ {'[',  '[',  '[',  '[',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  '[',  '[',  '[',  '[',  '[',  '[',  ']',  ']'},
	/* GEQ	*/ {'[',  '[',  '[',  '[',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  '[',  '[',  '[',  '[',  '[',  '[',  ']',  ']'},
	/* EQ	*/ {'[',  '[',  '[',  '[',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  '[',  '[',  '[',  '[',  '[',  '[',  ']',  ']'},
	/* NEQ	*/ {'[',  '[',  '[',  '[',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  '[',  '[',  '[',  '[',  '[',  '[',  ']',  ']'},
	/* PLUS */ {'[',  '[',  '[',  '[',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  '[',  '[',  '[',  '[',  ']',  ']'},
	/* MINUS*/ {'[',  '[',  '[',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  '[',  '[',  '[',  '[',  ']',  ']'},
	/* AST	*/ {'[',  '[',  '[',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  '[',  ']',  ']'},
	/* SL	*/ {'[',  '[',  '[',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  '[',  ']',  ']'},
	/* DSL	*/ {'[',  '[',  ' ',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  '[',  ']',  ']'},
	/* LPA	*/ {'[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '=',  ' '},
	/* RPA	*/ {' ',  ' ',  ' ',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ' ',  ']',  ']'},
	/* "$"	*/ {'[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  ' ',  ' '}};

// prevede token na vstupu na koordinat (taky pouzito pro flush v PA)
int decode(e_type token)
{
	switch (token)
	{
	case ID:
		return 0;
	case INT:
		return 1;
	case DEC:
		return 2;
	case STR:
		return 3;
	case L:
		return 4;
	case LEQ:
		return 5;
	case G:
		return 6;
	case GEQ:
		return 7;
	case EQ:
		return 8;
	case NEQ:
		return 9;
	case PLUS:
		return 10;
	case MINUS:
		return 11;
	case AST:
		return 12;
	case SL:
		return 13;
	case DSL:
		return 14;
	case LPA:
		return 15;
	case RPA:
		return 16;
	default:
		return 17; // "$"
	}
}

// vrati znak pro vyhodnoceni PA. isFin se nastavi na 1 pokud byl prichozi token dekodovan jako non-expr
char getFromTable(Token *tokenS, Token *tokenI, int *isFin)
{
	if (decode(tokenI->type) == 17) *isFin = 1;
	else *isFin = 0;

	return PATable[(tokenS == NULL) ? 17 : decode(tokenS->type)][decode(tokenI->type)];
}

#endif

// ALL IS DONE

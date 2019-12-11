#include "PATable.h"

#ifndef _PA_TABLE
#define _PA_TABLE

#define PA_SIZE 19

// tabulka precedencni analyzy
const char PATable[PA_SIZE][PA_SIZE] = {
	         // ID    INT   DEC   STR   L     LEQ   G     GEQ   EQ    NEQ   PLUS  MINUS AST   SL    DSL   LPA   RPA   NONE  "$"
	/* ID	*/ {' ',  ' ',  ' ',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ' ',  ']',  ' ', ']'},
	/* INT	*/ {' ',  ' ',  ' ',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ' ',  ']',  ' ', ']'},
	/* DEC	*/ {' ',  ' ',  ' ',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  '4',  ' ',  ']',  ' ', ']'},
	/* STR 	*/ {' ',  ' ',  ' ',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  '4',  '4',  '4',  '4',  ' ',  ']',  ' ', ']'},
	/* L	*/ {'[',  '[',  '[',  '[',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  '[',  '[',  '[',  '[',  '[',  '[',  ']',  '4', ']'},
	/* LEQ	*/ {'[',  '[',  '[',  '[',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  '[',  '[',  '[',  '[',  '[',  '[',  ']',  '4', ']'},
	/* G	*/ {'[',  '[',  '[',  '[',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  '[',  '[',  '[',  '[',  '[',  '[',  ']',  '4', ']'},
	/* GEQ	*/ {'[',  '[',  '[',  '[',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  '[',  '[',  '[',  '[',  '[',  '[',  ']',  '4', ']'},
	/* EQ	*/ {'[',  '[',  '[',  '[',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  '[',  '[',  '[',  '[',  '[',  '[',  ']',  '[', ']'},
	/* NEQ	*/ {'[',  '[',  '[',  '[',  ' ',  ' ',  ' ',  ' ',  ' ',  ' ',  '[',  '[',  '[',  '[',  '[',  '[',  ']',  '[', ']'},
	/* PLUS */ {'[',  '[',  '[',  '[',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  '[',  '[',  '[',  '[',  ']',  '4', ']'},
	/* MINUS*/ {'[',  '[',  '[',  '4',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  '[',  '[',  '[',  '[',  ']',  '4', ']'},
	/* AST	*/ {'[',  '[',  '[',  '4',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  '[',  ']',  '4', ']'},
	/* SL	*/ {'[',  '[',  '[',  '4',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  '[',  ']',  '4', ']'},
	/* DSL	*/ {'[',  '[',  '4',  '4',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  '[',  ']',  '4', ']'},
	/* LPA	*/ {'[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '=',  '[', ' '},
	/* RPA	*/ {' ',  ' ',  ' ',  ' ',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ']',  ' ',  ']',  ' ', ']'},
	/* NONE	*/ {' ',  ' ',  ' ',  ' ',  '4',  '4',  '4',  '4',  ']',  ']',  '4',  '4',  '4',  '4',  '4',  ' ',  ']',  ' ', ']'},
	/* "$"	*/ {'[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  '[',  ' ',  '[', ']'}};

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
	case NONE:
		return 17;
	default:
		return PA_SIZE - 1; // "$"
	}
}

// vrati znak pro vyhodnoceni PA. isFin se nastavi na 1 pokud byl prichozi token dekodovan jako non-expr
char getFromTable(Token *tokenS, Token *tokenI, int *isFin)
{
	if (decode(tokenI->type) == PA_SIZE-1) *isFin = 1;
	else *isFin = 0;

	return PATable[(tokenS == NULL) ? PA_SIZE-1 : decode(tokenS->type)][decode(tokenI->type)];
}

#endif

// ALL IS DONE

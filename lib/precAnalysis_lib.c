#include "symbolStack.h"

#define A 1
#define B 1

// asks scanner for the next token
char getNextToken() 
{
	return '0';
}


typedef struct patoken {
	char value;
	int isTerm;
} PAToken;


// translates invalid tokens into '$'
PAToken makePAToken(char token)
{
	PAToken out;
	out.value = token;
	out.isTerm = 0;			// TODO determine terminal
}

static char PATable[A][B] = {
	{ '=' }
};

char getPATableItem(char a, char b);
{
	return PATable[0][0];
}

void main()
{
	// PREC, ANALYSIS BEGINS HERE

	SStack *s;

	SSInit(s);

	SSPush(s, '$', 1);

	do {

		char a = SSClosestTerminal(s);
		PAToken b = makePAToken(getNextToken()); // TODO maybe?

		switch (getPATableItem(SSClosestTerminal(s)), b.value) {
			case '<': 
				
				
				break;

			case '=':
				SSPush(s, b.value, b.isTerm);
				b = makePAToken(getNextToken());				
				break;

			case '>': break;
			default: break;
		}


	} while ( SSClosestTerminal(s) != '$' || c != '$' );
}


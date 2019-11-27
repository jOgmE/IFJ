#include "PATable.c"
#include "PATokenStack_lib.c"
#include "token.h"

// TODO file what file?

Token expressionAnalysis ( Token input ) {

	PAStack *s;
	PAInit(&s);
	PAPushFin(s);

	Token curToken = input;
	int *isFin;

	do {
		switch (getFromTable(PATop(s), curToken)) {

			case '<':
				PAPushTerm(s, curToken);
				getToken(f, curToken);
			case '=':
				PAAddBracket(s);
				PAPushTerm(s, curToken);
				getToken(f, curToken);
			case '>':
				PAApplyRule(s);	// <- tady je moloch
			default:
				// TODO error

		}

	} while ( PATop(s) != FIN || !(*isFin) );

	return curToken;
}

#include "lib/PATable.h"
#include "lib/PACharStack_lib.h"
#include "scanner/token.h"

// TODO file what file?

Token *expressionAnalysis ( Token *input1, Token *input2, Token *res ) {

	PAStack *socket;
	PAInit(&socket);

	if ( *input2 != NULL ) PAPush(socket, input2);
	PAPush(socket, input1);	

	PAStack *s;
	PAInit(&s);
	PAPushFin(s);

	Token *curToken = ( *input2 == NULL ) input1 : input2;

	int *isFin;

	do {

		switch (getFromTable(PATopTerm(s), curToken)) {

			case '[':
				PAPushTerm(s, c, curToken);
				if (PATop(socket) != NULL) {
					curToken = PATop(socket);
					PAPop(socket);	
				} else {
					getToken(curToken);
				}
				break;

			case '=':
				PAAddBracket(s);
				PAPushTerm(s, c, curToken);
				if (PATop(socket) != NULL) {
					curToken = PATop(socket);
					PAPop(socket);	
				} else {
					getToken(curToken);
				}
				break;

			case ']':
				PAApplyRule(s, res);	// <- tady je moloch
				break;

			default:
				// TODO error
				break;

		}

	} while ( PATop(s) != FIN || !(*isFin) );

	PAYeet(socket);
	PAYeet(s);

	return curToken;
}

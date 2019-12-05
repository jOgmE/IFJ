#include "precAnalysis_lib.h"

Token *expressionAnalysis(Token *input1, Token *input2, Token *res)
{

	PAStack *socket;
	PAInit(&socket);

	if (input2 != NULL)
		PAPush(socket, input2);
	PAPush(socket, input1);

	PAStack *s;
	PAInit(&s);
	PAPushFin(s);

	Token *curToken = (input2 == NULL) ? input1 : input2;

	int *isFin;
	*isFin = 0;

	do
	{

		switch (getFromTable(PATopTerm(s)->type, curToken->type, isFin))
		{

		case '[':
			PAPush(s, curToken);
			if (PATop(socket) != NULL)
			{
				curToken = PATop(socket);
				PAPop(socket);
			}
			else
			{
				getToken(curToken);
			}
			break;

		case '=':
			PAAddBracket(s);
			PAPush(s, curToken);
			if (PATop(socket) != NULL)
			{
				curToken = PATop(socket);
				PAPop(socket);
			}
			else
			{
				getToken(curToken);
			}
			break;

		case ']':
			PAApplyRule(s, res); // <- tady je moloch
			break;

		default:
			// TODO error
			break;
		}

	} while (PAEndAtTop(s) || !(*isFin));

	PAYeet(socket);
	PAYeet(s);

	return curToken;
}

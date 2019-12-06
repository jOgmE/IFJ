#include "precAnalysis_lib.h"

// struktura prec analyzy je v podstate totozna s jejim popisem v prezentaci
// (dokonce i zasobnik pouziva znakove reprezentace 'E', 'i', '+' za operatory, '$' atd)
// Vetsina funkcionality se ukryva ve funkci PAApplyRule(), kterou nelze prehlednout
// v "precAnalysis_lib.h"

Token *expressionAnalysis(Token *input1, Token *input2, Token *res)
{

	// socket stack pro akomodaci vice prichazejicich vstupu
	PAStack *socket;
	PAInit(&socket);

	if (input2 != NULL)
		PAPush(socket, input2);
	PAPush(socket, input1);


	// inicializace hlavniho stacku PA
	PAStack *s;
	PAInit(&s);
	PAPushFin(s);

	
	Token *curToken = init_token();	
	curToken = (input2 == NULL) ? input1 : input2;

	int *isFin;
	*isFin = 0;



	do {
		// F L U S H
		if (kill_after_analysis == true) {
			
			while (decode(curToken->type)!=17)
			{
				if (PATop(socket) != NULL) {
					curToken = PATop(socket);
					PAPop(socket);	
				} else {
					getToken(curToken);
				}
			}
		
			break;
		}


		// vybere z tabulky PA prislusny symbol
			
		char switchC = getFromTable(PATopTerm(s), curToken, isFin);
		printf("%c\n", switchC); // TODO odstran
		switch (switchC) {

			case '[':
				PAPush(s, curToken);
				if (PATop(socket) != NULL) {
					curToken = PATop(socket);
					PAPop(socket);	
				} else {
					getToken(curToken);
				}
				break;

			case '=':
				PAAddBracket(s);
				PAPush(s, curToken);
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

			// kdyby scanner narazil na token mimo rozsah PA, je interpretovan jako '$', ktery ma sve ']'
			// a tim padem nemusi vylozene prechazet na chybu
			default:
				kill_after_analysis = true;
				global_error_code = SYNTAX_ANALYSIS_ERROR;
				print_compile_error(SYNTAX_ANALYSIS_ERROR, ERROR, line_count, "Neplatny vyraz.");
				break;
		
		}

	} while ( PAEndAtTop(s) || !(*isFin) );

	printf("Prefree.\n");

	// uvolneni -- NECO V YEETU TROPI POTIZE, TODO vyresit to
	//PAYeet(socket);
	//PAYeet(s);

	printf("Bye.\n");
	// navrat navic prejateho tokenu (interpretovaneho jako '$')
	return curToken;
}

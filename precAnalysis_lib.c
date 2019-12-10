#include "precAnalysis_lib.h"

extern Token *fake_token();
//#define getToken() fake_token()

// struktura prec analyzy je v podstate totozna s jejim popisem v prezentaci
// (dokonce i zasobnik pouziva znakove reprezentace 'E', 'i', '+' za operatory, '$' atd)
// Vetsina funkcionality se ukryva ve funkci PAApplyRule(), kterou nelze prehlednout
// v "precAnalysis_lib.h"

Token *expressionAnalysis(Token *input1, Token *input2, Token *res)
{
	if (decode(input1->type)==17) return input1;	// nic neprovedeno

	int tFlag = (input2 != NULL);	

	// inicializace hlavniho stacku PA
	PAStack *s;
	PAInit(&s);
	PAPushFin(s);

	
	
	Token *curToken = init_token();	
	curToken = input1;

	int isFin = 0;
	int tempInt = 0;


	do {
		// F L U S H
		if (kill_after_analysis == true) {
			
			while (decode(curToken->type)!=17)
			{
				if (tFlag) { 
					curToken = input2; tFlag = 0;
				}
				else curToken = getToken();
			}
		
			break;
		}


		// vybere z tabulky PA prislusny symbol
			
		char switchC = getFromTable(PATopTerm(s)->content, curToken, &isFin);
		
		switch (switchC) {

			case '=':
				PAPush(s, curToken);
				if (tFlag) { curToken = input2; tFlag = 0; }
				else curToken = getToken();
				break;

			case '[':
				PAAddBracket(s);
				PAPush(s, curToken);
				if (tFlag) { curToken = input2; tFlag = 0; }
				else curToken = getToken();
				break;

			case ']':
				if (PAApplyRule(s, res, &tempInt)) {
					kill_after_analysis = true;
					global_error_code = SYNTAX_ANALYSIS_ERROR;	
				};	// <- tady je moloch
				break;

			// kdyby scanner narazil na token mimo rozsah PA, je interpretovan jako '$', ktery ma sve ']'
			// a tim padem nemusi vylozene prechazet na chybu
			default:
				kill_after_analysis = true;
				global_error_code = SYNTAX_ANALYSIS_ERROR;
				print_compile_error(SYNTAX_ANALYSIS_ERROR, ERROR, line_count, "Neplatny vyraz.");
				break;
		
		}
		
	} while ( !(PAEndAtTop(s) && isFin) );


	PAYeet(s);

	if (!kill_after_analysis) changeLastRes(res);

	// navrat navic prejateho tokenu (interpretovaneho jako '$')
	return curToken;
}

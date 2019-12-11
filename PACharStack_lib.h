/** \file PACharStack_lib.h
 * Hlavicka knihovny pro praci se stackem precedencni analyzy. 
 *
 * \author Filip Draber (xdrabe09@stud.fit.vutbr.cz)
 * \date 2019-11-27
 */

#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "symtable.h"
#include "errors.h"
#include "adress_code.h"
#include "PATable.h"

#ifndef _PA_STACK_H_
#define _PA_STACK_H_


/** Struktura polozky stacku. */
typedef struct pastackelement
{
        char c; /**< reprezentace ve stacku */
        Token *content; /**< ukazatel na token ulozen ve stacku */
        struct pastackelement *belowPtr; /**< ukazatel na polozku nize */
} PAStackElem;

/** Struktura samotneho stacku. */
typedef struct pastack
{
        PAStackElem *top; /**< Ukazatel na vrchni polozku stacku */
} PAStack;


/** Inicializuje stack.
 * @param s Ukazatel na ukazatel na polozku stacku.
 */
void PAInit(PAStack **s);


/** Vlozi '$' na stack.
 * @param s Ukazatel na stack.
 */
void PAPushFin(PAStack *s);


/** Vlozi token na stacka a nastavi jeho reprezentaci.
 * @param s Ukazatel na stack.
 * @param content Ukazatel na token, ktery ma byt vlozen do stacku.
 */
void PAPush(PAStack *s, Token *content);


/** Vrati ukazatel na nejvyssi polozku stacku, ve kterm se nachazi terminal.
 * @param s Ukazatel na stack.
 *
 * @return ukazatel na nejvyssi terminal nebo NULL, pokud tam zadny term neni.
 */
PAStackElem *PATopTerm(PAStack *s);


/** Overi, jestli na vrcholu stacku neni '$'.
 * @param s Ukazatel na stack.
 *
 * @return int=0, pokud neni, a int=1 pokud je
 */
int PAEndAtTop(PAStack *s);


/** Vrati ukazatel na token ulozeny v nejvyssi polozce stacku.
 * @param s Ukazatel na stack.
 *
 * @return ukazatel na token na vrsku stacku nebo NULL, pokud je stack prazdny
 */
Token *PATop(PAStack *s);


/** Vlozi na stack '['.
 * @param s Ukazatel na stack.
 */
void PAAddBracket(PAStack *s);


/** Aplikuje nejake z pravidel na stack.
 * @param s Ukazatel na stack.
 * @param tempInt Ukazatel na pocitadlo, ktere rozsiruje ID docasnych tokenu.
 *
 * @return int=0 pokud je operace uspesna a int=1 pokud ne.
 */
int PAApplyRule(PAStack *s, int *tempInt);


/** Odstrani a uvolni vrchni polozku stacku.
 * @param s Ukazatel na stack.
 */
void PAPop(PAStack *s);

/** Smaze a uvolni cely stack.
 * @param s Ukazatel na stack.
 */
void PAYeet(PAStack *s);

#endif

/** \file precAnalysis_lib.h
 * Hlavicka funkce volane pro zpracovani vyrazu precedencni analyzou.
 *
 * \author Filip Draber (xdrabe09@stud.fit.vutbr.cz)
 * \date 2019-11-27
 */

#include "PATable.h"
#include "PACharStack_lib.h"
#include "token.h"
#include "scanner.h"

#ifndef _PREC_AN_
#define _PREC_AN_


/** Prubezne provedeni precedencni analyzy, dokud volani nenarazi na token spadajici mimo vyraz.
 * @param input1 Ukazatel na prvni token vyrazu.
 * @param input2 Ukazatel na druhy token vyrazu (ktery mohl byt omylem nacten v ramci synt. analyzy).
 * @param res Ukazatel na token finalniho vzsledku analyzy.
 * 
 * @return Ukazatel na omylem nacteny token ktery jiz nepatri do vyrazu.
 */
Token *expressionAnalysis ( Token *input1, Token *input2, Token *res);

#endif

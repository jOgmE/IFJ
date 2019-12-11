/** \file PATable.h
 * Hlavicka pro knihovnu pro praci s tabulkou precedencni analyzy.
 *
 * \author Filip Draber (xdrabe09@stud.fit.vutbr.cz)
 * \date 2019-11-27
 */

#include "token.h"

#ifndef _TABLE_H_
#define _TABLE_H

/** Vrati char pro rozhodovani v ramci precedencni analyzy.
 * @param tokenS Ukazatel na nejvyssi terminal na stacku.
 * @param tokenI Ukazatel na nejnovejsi ziskany token.
 * @param isFin Ukazatel na integerovy flag ktery naznacuje nalezeni tokenu '$' (0: nenalezen, 1: nalezen)
 *
 * @return char '[', '=', ']', ' ', podle kteroho se rozhoduje precedencni analyza.
 */
char getFromTable ( Token *tokenS, Token *tokenI, int *isFin);


/** Dekoduje token podle typu na index PA tabulky.
 * @param token Typ tokenu.
 *
 * return int oznacujici index radku/sloupce daneho typu tokenu v PA tabulce.
 */
int decode ( e_type token );

#endif

/** @incednt_stack.h
 * .h soubor k funkcím pro indent stack
 *
 * @author xstrna14
 * @version 1.0
 * @date 3.11.2019
 */

#ifndef _INDENT_STACK_H_
#define _INDENT_STACK_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

/**
*Struktura na položku v zásobníku
*/
typedef struct tIndentStackItem {
  unsigned indent;          /**< Hodnota položky*/
  struct tIndentStackItem *prev;  /**< Ukazatel na předchozí položku v zásobníku*/
} tIndentStackItem;

/**
*Struktura zásobníku*/
typedef struct {
  struct tIndentStackItem *top; /**<Ukazatel na položku na vrcholu zásobníku*/
  size_t size;            /**Aktuální velikost zásobníku*/
} tIndentStack;

/**
*Inicializuje zásobník a první položku s hodnotou 0, která nemůže být změněna.
*@return tIndentStack* Ukazatel na stack.
*/
tIndentStack *indentStackInit();

/**
*Kontroluje, zda-li je prázdný zásobník
*@param tIndentStack* ukazatel na kontrolovaný zásobník
*@return true je-li zásobník prázdný (aka obsahuje jen jeden item s hodnotou 0)
*@return false obsahuje-li zásobník ještě jinou položku krom 0
*/
bool indentStackEmpty(tIndentStack *);

/**
*Přidá položku typu unsigned na vrchol zásobníku
*(a změní hodnoty zásobníku, aby odpovídaly aktuálnímu stavu)
*@param tIndentStack* ukazatel na zásobník
*@param unsigned číslo k přidání
*/
void indentStackPush(tIndentStack *, unsigned);

/**
*Vrátí hodnotu vrchní položky zásobníku
*@param tIndentStack* ukazatel na zásobník
*@return unsigned hodnota vrchní položky zásbníku
*/
unsigned indentStackTop(tIndentStack *);

/**
*Odstraní vrchní položku zásobníku (a upraví všechny závislosti)
*@param tIndentStack* ukazatel na zásobníku
*/
void indentStackPop(tIndentStack *);

/**
*Porovná hodnotu položky na vrcholu zásobníku a zadané číslo
*@param tIndentStack* ukazatel na zásobník
*@param unsigned číslo k porovnání
*@return 0 číslo má stejnou hodnotu jako položka na vrcholu
*@return 1 číslo je větší než položka na vrcholu zásobníku
*@return -1 číslo je menší než položka na vrcholu zásobníku
*/
int indentStackTopCompare(tIndentStack *, unsigned);

/**
*Zničí zásobník, uvolní všechny položky
*@param tIndentStack* ukazatel na zásobník
*/
void indentStackDestroy(tIndentStack *);

#endif //_INDENT_STACK_H_

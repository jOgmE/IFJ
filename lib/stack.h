/** @stack.h
 * .h soubor k funkcím prostack
 *
 * @author xstrna14
 * @version 2.0
 * @date 9.11.2019
 *
 * Update: Fixed the stack to be universal, not just for indent
 */

#ifndef _STACK_H_
#define _STACK_H_

#include <stdbool.h>
#include <stdio.h>

/**
*Struktura na položku v zásobníku
*/
typedef struct tStackItem {
  unsigned indent;          /**< Hodnota položky*/
  struct tStackItem *prev;  /**< Ukazatel na předchozí položku v zásobníku*/
} tStackItem;

/**
*Struktura zásobníku*/
typedef struct {
  struct tStackItem *top; /**<Ukazatel na položku na vrcholu zásobníku*/
  size_t size;            /**Aktuální velikost zásobníku*/
} tStack;

/**
*Inicializuje zásobníkovou hlavičku, stack->top = NULL
*@return tStack* Ukazatel na stack.
*/
tStack *stackInit();

/**
*Kontroluje, zda-li je prázdný zásobník
*@param tStack* ukazatel na kontrolovaný zásobník
*@return true je-li zásobník prázdný
*@return false obsahuje-li zásobník aspoň jednu položku
*/
bool stackEmpty(tStack *);

/**
*Přidá položku typu unsigned na vrchol zásobníku
*(a změní hodnoty zásobníku, aby odpovídaly aktuálnímu stavu)
*@param tStack* ukazatel na zásobník
*@param unsigned číslo k přidání
*/
void stackPush(tStack *, unsigned);

/**
*Vrátí hodnotu vrchní položky zásobníku
*@param tStack* ukazatel na zásobník
*@return unsigned hodnota vrchní položky zásbníku
*/
unsigned stackTop(tStack *);

/**
*Odstraní vrchní položku zásobníku (a upraví všechny závislosti)
*@param tStack* ukazatel na zásobníku
*/
void stackPop(tStack *);

/**
*Porovná hodnotu položky na vrcholu zásobníku a zadané číslo
*@param tStack* ukazatel na zásobník
*@param unsigned číslo k porovnání
*@return 0 číslo má stejnou hodnotu jako položka na vrcholu
*@return 1 číslo je větší než položka na vrcholu zásobníku
*@return -1 číslo je menší než položka na vrcholu zásobníku
*/
int stackTopCompare(tStack *, unsigned);

/**
*Porovná velikost zásobníku se zadaným číslem.
*@param tStack* ukazatel na zásobník
*@param size_t porovnavane cislo
*@return true stack je zadané velikosti
*@return false stack není zadané velikosti
*/
bool stackSizeIs(tStack *, size_t);

/**
*Zničí zásobník, uvolní všechny položky
*@param tStack* ukazatel na zásobník
*/
void stackDestroy(tStack *);

#endif //_INDENT_STACK_H_

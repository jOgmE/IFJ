/** @sa_stack.h
* Funkce pro stack pro syntaktickou analyzu
*
* @author xstrna14
* @version 1.0
* @date 10.11.2019
*/

#ifndef _SA_STACK_H
#define _SA_STACK_H

#include <stdbool.h>
#include <stdio.h>
//#include "error.h"

#define MAX_ITEM_LENGHT //max delka polozky pouzite v teto gramatice

/**
*Struktura na položku v zásobníku
*/
typedef struct tSAStackItem {
  char data[MAX_ITEM_LENGHT+1]; /**< Hodnota položky*/
  struct tSAStackItem *prev;  /**< Ukazatel na předchozí položku v zásobníku*/
} tSAStackItem;

/**
*Struktura zásobníku
*/
typedef struct {
  struct tSAStackItem *top; /**< Ukazatel na položku na vrcholu zásobníku*/
  size_t size;              /**< Aktuální velikost zásobníku*/
} tSAStack;

tSAStack *SAStackInit();
bool SAStackEmpty(tSAStack *);
void SAStackPush(tSAStack *, char *);


#endif //_SA_STACK_H

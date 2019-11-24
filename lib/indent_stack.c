/** @indent_stack.c
 * Funkce pro práce s indent_stack
 *
 * @author xstrna14
 * @version 1.1
 * Přejmenování kvůli ostatním stack funkcím
 * @date 9.11.2019
 */

#include "indent_stack.h"

void indentStackPush(tIndentStack *s, unsigned n)
{
    tIndentStackItem *to_be_added = malloc(sizeof(tIndentStackItem));
    if(to_be_added == NULL) {
      /*TODO uncomment when error library finished
      print_internal_error(INTERNAL_ERROR, ERROR, "Interní chyba alokace paměti pro indent_stack scanneru.\n");*/
      global_error_code = INTERNAL_ERROR;
      return;
    }
    to_be_added->prev = s->top;
    to_be_added->indent = n;
    (s->size)++;
    s->top = to_be_added;
}

tIndentStack *indentStackInit()
{
  tIndentStack* stack = malloc(sizeof(tIndentStack));
  if(stack == NULL) {
    /*TODO uncomment when error library finished
    print_internal_error(INTERNAL_ERROR, ERROR, "Interní chyba alokace paměti pro indent_stack scanneru.\n");*/
    global_error_code = INTERNAL_ERROR;
    return NULL;
  }
  stack->size = 0;
  stack->top = NULL;
  indentStackPush(stack, 0);
  return stack;
}

bool indentStackEmpty(tIndentStack *s)
{
  if(s->size == 1) return true;
  return false;
}


unsigned indentStackTop(tIndentStack *s)
{
  return s->top->indent;
}

void indentStackPop(tIndentStack *s)
{
  if(s->size == 1) return; //na pocatecni 0 nesaham
  tIndentStackItem *to_be_destroyed = s->top;
  s->top = s->top->prev;
  (s->size)--;
  free(to_be_destroyed);
}

int indentStackTopCompare(tIndentStack *s, unsigned n)
{
  return((indentStackTop(s) == n) ? 0 : (n > indentStackTop(s) ? 1 : -1));
}

void indentStackDestroy(tIndentStack *s)
{
  tIndentStackItem *curr_item = s->top;
  tIndentStackItem *prev_item;
  while(curr_item != NULL) {
    prev_item = curr_item->prev;
    free(curr_item);
    curr_item = prev_item;
  }
  free(s);
}

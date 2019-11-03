/** @indent_stack.c
 * Funkce pro práce s indent_stack
 *
 * @author xstrna14
 * @version 1.0
 * @date 3.11.2019
 */

#include "indent_stack.h"
#include <stdlib.h>

void stackPush(tIndentStack *s, unsigned n)
{
    tStackItem *to_be_added = malloc(sizeof(tStackItem));
    if(to_be_added == NULL) {
      //TODO allocation error handling
      //TODO what if at top size of size_t?
      return;
    }
    to_be_added->prev = s->top;
    to_be_added->indent = n;
    (s->size)++;
    s->top = to_be_added;
}

tIndentStack *stackInit()
{
  tIndentStack* stack = malloc(sizeof(tIndentStack));
  if(stack == NULL) return NULL;
  stack->size = 0;
  stack->top = NULL;
  stackPush(stack, 0);
  return stack;
}

bool stackEmpty(tIndentStack *s)
{
  if(s->size == 1) return true;
  return false;
}


unsigned stackTop(tIndentStack *s)
{
  return s->top->indent;
}

void stackPop(tIndentStack *s)
{
  if(s->size == 1) return; //na pocatecni 0 nesaham
  tStackItem *to_be_destroyed = s->top;
  s->top = s->top->prev;
  (s->size)--;
  free(to_be_destroyed);
}

int stackTopCompare(tIndentStack *s, unsigned n)
{
  return((stackTop(s) == n) ? 0 : (n > stackTop(s) ? 1 : -1));
}

void stackDestroy(tIndentStack *s)
{
  tStackItem *curr_item = s->top;
  tStackItem *prev_item;
  while(curr_item != NULL) {
    prev_item = curr_item->prev;
    free(curr_item);
    curr_item = prev_item;
  }
  free(s);
}

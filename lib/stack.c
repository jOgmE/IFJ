/** @stack.c
 * Funkce pro práce s stack
 *
 * @author xstrna14
 * @version 2.0
 * @date 9.11.2019
 *
 * UPDATE 2.0: fixed to be universal
 */

#include "stack.h"
#include <stdlib.h>

tStack *stackInit()
{
  tStack* stack = malloc(sizeof(tStack));
  if(stack == NULL) return NULL;
  stack->size = 0;
  stack->top = NULL;
  return stack;
}

bool stackEmpty(tStack *s)
{
  if(s->size == 0) return true;
  return false;
}

void stackPush(tStack *s, unsigned n)
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

unsigned stackTop(tStack *s)
{
  return s->top->indent;
}

void stackPop(tStack *s)
{
  if(s->size == 0) return; //na pocatecni 0 nesaham
  tStackItem *to_be_destroyed = s->top;
  s->top = s->top->prev;
  (s->size)--;
  free(to_be_destroyed);
}

int stackTopCompare(tStack *s, unsigned n)
{
  return((stackTop(s) == n) ? 0 : (n > stackTop(s) ? 1 : -1));
}

void stackDestroy(tStack *s)
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

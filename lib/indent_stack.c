//TODO

#include "indent_stack.h"
#include <stdlib.h>

tIndentStack *stackInit()
{
  tIndentStack* stack = malloc(sizeof(tIndentStack));
  if(stack == NULL) return NULL;
  stack->size = 0;
  stack->top = NULL;
  return stack;
}

bool stackEmpty(tIndentStack *s)
{

}

void stackPush(tIndentStack *s, unsigned n)
{

}

unsigned stackTop(tIndentStack *s)
{

}

void stackPop(tIndentStack *s)
{

}

int stackTopCompare(tIndentStack *s, unsigned n)
{

}

void stackDestroy(tIndentStack *s)
{

}

//TODO

#ifndef _INDENT_STACK_H_
#define _INDENT_STACK_H_

#include <stdbool.h>

typedef struct tStackItem {
  unsigned indent;
  tStackItem *prev;
};

typedef struct {
  tStackItem *top;
  size_t size;
} tIndentStack;


tIdentStack *stackInit();

bool stackEmpty(tIdentStack *);

void stackPush(tIndentStack *, unsigned);

unsigned stackTop(tIdentStack *);

void stackPop(tIdentStack *);

bool stackTopCompare(tIndentStack *, unsigned);

void stackDestroy(tIndentStack *);

#endif //_INDENT_STACK_H_

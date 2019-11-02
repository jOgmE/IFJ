//TODO

#ifndef _INDENT_STACK_H_
#define _INDENT_STACK_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct tStackItem {
  unsigned indent;
  struct tStackItem *prev;
};

typedef struct {
  struct tStackItem *top;
  size_t size;
} tIndentStack;


tIndentStack *stackInit();

bool stackEmpty(tIndentStack *);

void stackPush(tIndentStack *, unsigned);

unsigned stackTop(tIndentStack *);

void stackPop(tIndentStack *);

bool stackTopCompare(tIndentStack *, unsigned);

void stackDestroy(tIndentStack *);

#endif //_INDENT_STACK_H_

//TODO

#ifndef _INDENT_STACK_H_
#define _INDENT_STACK_H_

typedef struct tStackItem {
  int indent;
  tStackItem *prev;
};

typedef struct {
  tStackItem *top;
  size_t size;
} tIndentStack;

tIdentStack *stackInit();


#endif //_INDENT_STACK_H_

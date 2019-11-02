//TODO

#include "indent_stack.h"

tIndentStack *stackInit();

bool stackEmpty(tIndentStack *);

void stackPush(tIndentStack *, unsigned);

unsigned stackTop(tIndentStack *);

void stackPop(tIndentStack *);

bool stackTopCompare(tIndentStack *, unsigned);

void stackDestroy(tIndentStack *);

//TODO

#include "indent_stack.h"

tIdentStack *stackInit();

bool stackEmpty(tIdentStack *s);

void stackPush(tIndentStack *s, unsigned i);

unsigned stackTop(tIdentStack *s);

void stackPop(tIdentStack *s);

bool stackTopCompare(tIndentStack *s, unsigned i);

void stackDestroy(tIndentStack *s);

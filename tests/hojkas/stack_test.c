//TODO

#include "indent_stack.h"

void printStack(tIndentStack *s)
{
  printf("Stack state (top to bottom): ");
  tStackItem *curr_item = s->top;
  if(curr_item == NULL) printf("Stack empty\n");
  while(curr_item != NULL) {
    printf("%u ", curr_item->indent);
    curr_item = curr_item->prev;
  }
}

int main()
{
  printf("-----TESTS_BEGIN-----\n\n");

  tIndentStack *s = stackInit();
  printStack(s);

  /*
  stackPush(s, 0);
  stackPush(s, 2);
  stackPush(s, 4);

  printf("Pushed 0, 2, 4\n");
  printStack(s);

  printf("On top: %u\n", stackTop(s));

  stackPop(s);
  printf("On top: %u\n", stackTop(s));
  stackPop(s);
  printf("Stack empty? %d", stackEmpty(s));
  printf("On top: %u\n", stackTop(s));
  stackPop(s);
  printf("Stack empty? %d", stackEmpty(s));
  printStack(s);

  stackPush(s, 0);
  stackPush(s, 2);
  stackPush(s, 4);

  printStack(s);
  printf("Compare top with 6: %d\n", stackTopCompare(s, 6));
  printf("Compare top with 4: %d\n", stackTopCompare(s, 4));
  printf("Compare top with 2: %d\n", stackTopCompare(s, 2));

  stackDestroy(s);
  */
  return 0;
}

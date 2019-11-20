/** \indentStack_test.c
 * Základní testy pro indent indentStack
 *
 * \author xstrna14
 * \version 1.0
 * \date 3.11.2019
 */

#include "indent_stack.h"

void printindentStack(tIndentStack *s)
{
  printf("stack state (top to bottom): ");
  tIndentStackItem *curr_item = s->top;
  if(curr_item == NULL) printf("stack empty");
  while(curr_item != NULL) {
    printf("%u ", curr_item->indent);
    curr_item = curr_item->prev;
  }
  printf("\n");
}

int main()
{
  printf("-----TESTS_BEGIN-----\n\n");

  tIndentStack *s = indentStackInit();
  printindentStack(s);

  indentStackPush(s, 1);
  indentStackPush(s, 2);
  indentStackPush(s, 4);

  printf("Pushed 1, 2, 4\n");
  printindentStack(s);

  printf("On top: %u\n", indentStackTop(s));

  indentStackPop(s);
  printf("On top: %u\n", indentStackTop(s));
  indentStackPop(s);
  printf("indentStack empty? %d", indentStackEmpty(s));
  printf("On top: %u\n", indentStackTop(s));
  indentStackPop(s);
  printf("indentStack empty? %d", indentStackEmpty(s));
  printindentStack(s);

  indentStackPush(s, 0);
  indentStackPush(s, 2);
  indentStackPush(s, 4);

  printindentStack(s);
  printf("Compare top with 6: %d\n", indentStackTopCompare(s, 6));
  printf("Compare top with 4: %d\n", indentStackTopCompare(s, 4));
  printf("Compare top with 2: %d\n", indentStackTopCompare(s, 2));

  indentStackDestroy(s);

  return 0;
}

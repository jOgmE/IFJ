//parser testing yay finally

#include "../../parser.h"
#include "symtable.h"
#include "dynamic_string.h"

FILE *f = NULL; //scaner to jeste nema

typedef enum {
  L_WHILE, L_IF, L_ELSE, L_END
} label_enum;
/*
cstring* create_label(label_enum type)
{
  static unsigned while_count = 0;
  static unsigned if_count = 0;

  switch (type)
  {
    case L_WHILE:s

      break;
    case L_IF:

      break;
  }
}
*/
int main()
{



  parser_do_magic();
  return 0;
}

//parser testing yay finally

#include "../../parser.h"
#include "symtable.h"
#include "dynamic_string.h"

FILE *source_file = NULL; //scaner to jeste nema


int main()
{
  source_file = fopen("input", "r");
  if(source_file == NULL) {
    fprintf(stderr, "well, file fault\n");
    return 1;
  }
  parser_do_magic();
  fclose(source_file);
  return 0;
}

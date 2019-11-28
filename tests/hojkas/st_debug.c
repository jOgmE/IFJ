#include "../../lib/dynamic_string.h"
#include "../../lib/symtable.h"

extern STable* global_st;
extern STable* local_st;

void print_item(STItem *i)
{
  printf("   I   %d -   %d - %3ld - %d - ", i->type, i->defined, i->first_occur_line, i->number_of_params);
  const char *str = get_cstring_string(i->key);
  printf("%s\n", str);
}

void print_line(STItem *i)
{
  printf("LINE: type -D/N- FOL -NOP- key\n");
  if(i == NULL) printf("empty\n");
  while(i != NULL) {
    print_item(i);
    i = i->next;
  }
}

void print_table(STable *st, size_t size)
{
  printf("\nTABLE BEGIN\n");
  for(int i = 0; i < size; i++) print_line(st->item_array[i]);

  printf("TABLE END\n\n");
}

void print_global()
{
  print_table(global_st, global_st->size);
}

void print_local()
{
  print_table(local_st, local_st->size);
}

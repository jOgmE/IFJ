#include "symtable.h"
#include "dynamic_string.h"

extern STable* global_st;
extern STable* local_st;

//#define LOCAL_ST_SIZE 20
//#define GLOBAL_ST_SIZE 100

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
  printf("TABLE BEGIN\n");
  for(int i = 0; i < size; i++) print_line(st->item_array[i]);

  printf("TABLE END\n");
}

int main()
{
  start_symtable_with_functions();

  cstring* str = init_cstring("jedna");
  define_id_from_info(str, ST_INT, 0);

  print_table(global_st, 10);

  destroy_symtable(&global_st);
  return 0;
}

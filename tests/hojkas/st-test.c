#include "symtable.h"
#include "dynamic_string.h"

extern STable* global_st;
extern STable* local_st;

//#define LOCAL_ST_SIZE 20
//#define GLOBAL_ST_SIZE 100

FILE *f = NULL; //at scanner nerve

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

int main()
{
  start_symtable_with_functions();

  cstring* str = init_cstring("jedna");
  //define_id_from_info(str, ST_VALUE, 0);
  add_undef_id_from_info(str);
  str = init_cstring("ahoj)");
  define_id_from_info(str, ST_VALUE, 0);

  print_table(global_st, 10);
  if(search_st(global_st, str)) printf("string that shoud be there is there\n");
  else printf("whoopsie\n");
  str = init_cstring("dva");
  if(search_st(global_st, str)) printf("now this is bad\n");
  else printf("not there, yay\n");
  free_cstring(str);

  go_in_local();

  str = init_cstring("dva");
  define_id_from_info(str, ST_VALUE, 0);
  str = init_cstring("jedna");
  //define_id_from_info(str, ST_VALUE, 0);
  //add_undef_id_from_info(str);

  print_table(local_st, 5);

  go_in_global();

  define_id_from_info(str, ST_VALUE, 0);
  print_table(global_st, 10);

  clean_all_symtables();
  return 0;
}

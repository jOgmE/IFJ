/** @symtable.c
 * Tabulka symbolů řešená pomocí hash table
 *
 * @author xstrna14
 * @version 1.0
 * @date 24.11.2019
 */


#include "symtable.h"

STable* global_st = NULL;
STable* local_st = NULL;
bool in_global = true;

extern size_t line_count;
extern bool kill_after_analysis;

//TODO what values are optimal? I have no clue
#define LOCAL_ST_SIZE 5
#define GLOBAL_ST_SIZE 10

//TODO uncomment
//extern size_t line_count;

//*******************************
// TVORBA, ZMĚNA, ZÁNÍK TABULKY
//*******************************

unsigned hashCode(cstring *key)
{
  //TODO lepší funkci!!! a případně jak to vyřešit, když se to yoinkne?
  char *str = get_cstring_string(key);
  unsigned res = strlen(str) * str[0];
  return res;

  /*
  //Licence: žádná (Public Domain)
  //převzato z kostry IJC druhého projektu 2019, která je pod licencí Public Domain
  uint32_t h=0;     // musí mít 32 bitů
  const unsigned char p;
  for(p=(const unsigned char)str; p!='\0'; p++) h = 65599h + *p;
  return h;*/
  //konec převzaté části
}

void create_symtable(STable **st, size_t size)
{
  (*st) = malloc(sizeof(STable));
  if((*st) == NULL) goto table_alloc_error;
  (*st)->item_array = malloc(size * sizeof(STItem*));
  if((*st)->item_array == NULL) goto item_array_alloc_error;

  (*st)->size = size;

  for(int i = 0; i < size; i++) (*st)->item_array[i] = NULL;
  return;

  item_array_alloc_error:
    free((*st));
    //write message TODO
    //set flag
  table_alloc_error:
    //write message
    return;
}

void destroy_line(STItem **i)
{
  STItem *curr = *i;
  STItem *next = *i;
  while(curr != NULL) {
    next = curr->next;
    free_cstring(curr->key);
    free(curr);
    curr = next;
  }
}

void destroy_symtable(STable **st)
{
  size_t size = (*st)->size;
  for(int i = 0; i < size; i++) destroy_line(&((*st)->item_array[i]));
  free((*st)->item_array);
  free((*st));
}

STItem* init_st_item()
{
  STItem *new = malloc(sizeof(STItem));
  if(new == NULL) {
    //chyba aokace, TODO
    return NULL;
  }
  new->type = ST_UNDEFINED;
  new->defined = false;
  new->key = NULL;
  new->first_occur_line = 0;
  new->number_of_params = 0;
  new->next = NULL;
}

void append_item(STItem* item)
{
  unsigned line;

  if(in_global) {
    //jsme v global tabulce, vložím na začátek
    line = hashCode(item->key) % GLOBAL_ST_SIZE;
    item->next = global_st->item_array[line];
    global_st->item_array[line] = item;
  }
  else {
    //jsme v local tabulky, vložím na začátek
    line = hashCode(item->key) % LOCAL_ST_SIZE;
    item->next = local_st->item_array[line];
    local_st->item_array[line] = item;
  }
}

//*****************************
// POMOCNÉ FUNKCE NEVIDITELNÉ
//*****************************

bool search_st(STable *st, cstring *key)
{
  //TODO do something about it
  if(st == NULL) fprintf(stderr, "[hojkas] symtable.c: search_st(): Tabulka neexistuje\n");

  unsigned line = hashCode(key) % st->size;
  STItem *curr = st->item_array[line];
  bool found = false;
  while(curr != NULL) {
    if(compare_cstring(key, curr->key)) {
      found = true;
      break;
    }
    curr = curr->next;
  }
  return found;
}

//**************************
// PRÁCE SE SYMBOLY ZVNĚJŠÍ
//*************************

void start_symtable_with_functions()
{
  create_symtable(&global_st, GLOBAL_ST_SIZE);
  if(global_st == NULL) {
    //TODO chyba alokace
    return;
  }

  //load it with all premade functions
}

void go_in_local()
{
  //TODO just placeholder, this should NOT happen
  if(in_global == false) fprintf(stderr, "[hojkas] symtable.c: go_in_local(): def fce uvnitr def fce, ma byt osetreno jinde\n");
  in_global = false;
  create_symtable(&local_st, LOCAL_ST_SIZE);
}

void go_in_global()
{
  //TODO just placeholder, this should NOT happen
  if(in_global == true) fprintf(stderr, "[hojkas] symtable.c: go_in_global(): snaha vyskocit z funkce kdyz nejsme ve funkci\n");
  in_global = true;
  destroy_symtable(&local_st);
}

void define_id_from_token(Token *token, int param_count)
{
  //TODO
  if(token == NULL) fprintf(stderr, "[hojkas] symtable.c: define_id_from_token(): dostal empty token\n");

  if(in_global == false) {
    if(search_st(local_st, token->str)) {
      //token už definovan v loc_table


      return;
    }
  }

  if(search_st(global_st, token->str)) {
    //token už def v glob table

    return;
  }

  //vše okay, vytvoř idčko
  STItem *new = init_st_item();
  if(new == NULL) return;
  new->type = token->type;
  new->number_of_params = param_count;
  new->defined = true;
  //TODO nemůžu přímo. Jak? (nemůžu proto, že bych tabulku uvolňovala dříve, než token v ac zrecyklovaný)
  //new->key =
}

void define_id_from_info(cstring *id, st_type type, int param_count)
{
  //something that shouldn't but could go wrong?
  //search in local and global, if it is not already defined
  //if not, define it
  //if yes, error, print it, set stop after analysis, continue

  //serach first
  //errror if found

  STItem *new = init_st_item();
  if(new == NULL) return;
  new->key = id;
  new->type = type;
  //TODO uncomment
  //new->first_occur_line = line_count;
  new->number_of_params = param_count;
  new->defined = true;
  append_item(new);
}

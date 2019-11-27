/** @symtable.c
 * Tabulka symbolů řešená pomocí hash table
 *
 * @author xstrna14
 * @version 1.0
 * @date 24.11.2019
 */

//TODO když to najde error, nečistí to key string... asi i jinde
//prostě přidat, že když to key nezpracuje, ať ho to vymaže


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
  const char *str = get_cstring_string(key);
  unsigned res = 523*strlen(str) + 409*str[0];
  return res;

  /*
  //Licence: žádná (Public Domain)
  //převzato z kostry IJC druhého projektu 2019, která je pod licencí Public Domain
  uint32_t h=0;     // musí mít 32 bitů
  const unsigned char p;
  for(p=(const unsigned char)str; p!='\0'; p++) h = 65599h + *p;
  return h;*/
  //akorát nefunguje no :HAhaa:
  //konec převzaté části
}

void create_symtable(STable **st, size_t size)
{
  (*st) = malloc(sizeof(STable));
  if((*st) == NULL) {
    print_internal_error(INTERNAL_ERROR, ERROR, "Interni chyba alokace paměti pro symtable.c: create_symtable()\n");
    global_error_code = INTERNAL_ERROR;
    return;
  }
  (*st)->item_array = malloc(size * sizeof(STItem*));
  if((*st)->item_array == NULL) {
    free((*st));
    print_internal_error(INTERNAL_ERROR, ERROR, "Interni chyba alokace paměti pro symtable.c: create_symtable()\n");
    global_error_code = INTERNAL_ERROR;
    return;
  }

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
  *st = NULL;
}

STItem* init_st_item()
{
  STItem *new = malloc(sizeof(STItem));
  if(new == NULL) {
    print_internal_error(INTERNAL_ERROR, ERROR, "Interni chyba alokace paměti pro symtable.c: init_item()\n");
    global_error_code = INTERNAL_ERROR;
    return NULL;
  }
  new->type = ST_UNDEFINED;
  new->defined = false;
  new->key = NULL;
  new->first_occur_line = 0;
  new->number_of_params = 0;
  new->next = NULL;
  return new;
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

void no_check_def(cstring *key, st_type type, int param_count)
{
  STItem *new = init_st_item();
  if(new == NULL) return;
  new->key = key;
  new->type = type;
  new->first_occur_line = 0;
  if(type == ST_FUNCTION) new->number_of_params = param_count;
  new->defined = true;
  append_item(new);
}

//**************************
// PRÁCE SE SYMBOLY ZVNĚJŠÍ
//*************************

void start_symtable_with_functions()
{
  create_symtable(&global_st, GLOBAL_ST_SIZE);
  if(global_st == NULL) return; //error vyřešen ve volání create_symtable

  no_check_def(init_cstring("inputs"), ST_FUNCTION, 0);
  if(global_error_code == INTERNAL_ERROR) return;
  no_check_def(init_cstring("inputi"), ST_FUNCTION, 0);
  if(global_error_code == INTERNAL_ERROR) return;
  no_check_def(init_cstring("inputf"), ST_FUNCTION, 0);
  if(global_error_code == INTERNAL_ERROR) return;
  no_check_def(init_cstring("print"), ST_FUNCTION, -1);
  if(global_error_code == INTERNAL_ERROR) return;
  no_check_def(init_cstring("len"), ST_FUNCTION, 1);
  if(global_error_code == INTERNAL_ERROR) return;
  no_check_def(init_cstring("substr"), ST_FUNCTION, 3);
  if(global_error_code == INTERNAL_ERROR) return;
  no_check_def(init_cstring("ord"), ST_FUNCTION, 2);
  if(global_error_code == INTERNAL_ERROR) return;
  no_check_def(init_cstring("chr"), ST_FUNCTION, 1);
}

void go_in_local()
{
  //TODO just placeholder, this should NOT happen
  if(in_global == false) {
    fprintf(stderr, "[hojkas] symtable.c: go_in_local(): def fce uvnitr def fce, ma byt osetreno jinde\n");
    return;
  }
  in_global = false;
  create_symtable(&local_st, LOCAL_ST_SIZE);
}

void go_in_global()
{
  //TODO just placeholder, this should NOT happen
  if(in_global == true) {
    fprintf(stderr, "[hojkas] symtable.c: go_in_global(): snaha vyskocit z funkce kdyz nejsme ve funkci\n");
    return;
  }
  in_global = true;
  destroy_symtable(&local_st);
}

void define_id_from_info(cstring *key, st_type type, int param_count)
{
  //something that shouldn't but could go wrong?
  //search in local and global, if it is not already defined
  //if not, define it
  //if yes, error, print it, set stop after analysis, continue
  printf("am I in global? %d\n", in_global);
  unsigned line;
  STItem *curr;

  if(!in_global) {
    //prohledat lokální i globální
    if(local_st == NULL) fprintf(stderr, "[hojkas] symtable.c: define_id_from_info(): Tabulka neexistuje\n");

    line = hashCode(key) % local_st->size;
    curr = local_st->item_array[line];

    while(curr != NULL) {
      if(compare_cstring(key, curr->key)) {
        //nalezen key
        if(curr->defined == true) {
          //TODO
          kill_after_analysis = true;
          if(global_error_code == SUCCESS) global_error_code = SEMANTIC_DEFINITION_ERROR;
          fprintf(stderr, "placeholder: redefinition of local id with key %s\n", get_cstring_string(key));
          return;
        }

        //nalezen key ale neni definovan
        curr->defined = true;
        curr->type = type;
        curr->number_of_params = param_count;

        return;
      }
      curr = curr->next;
    }

    //dostali jsme-li se sem, polozka neni v loc, ale jeste muze byt koflikt s global
    //pokud je toto id v global, je to chyba
    if(search_st(global_st, key)) {
      //pruser, id v global je (:
      //TODO placeholder
      kill_after_analysis = true;
      if(global_error_code == SUCCESS) global_error_code = SEMANTIC_DEFINITION_ERROR;
      fprintf(stderr, "placeholder: redefinition of global id with key %s\n", get_cstring_string(key));
      return;
    }
    //pokud to nenaslo ani v local ani global, spadne to do vytvor id
  }
  else {
    //prohledat globalni
    //dostali-li jsme se sem, neexistuje loc tabulka, jsme v global
    if(global_st == NULL) fprintf(stderr, "[hojkas] symtable.c: define_id_from_info(): Tabulka neexistuje\n");

    line = hashCode(key) % global_st->size;
    curr = global_st->item_array[line];

    while(curr != NULL) {
      if(compare_cstring(key, curr->key)) {
        //nalezen key
        if(curr->defined == true) {
          //TODO
          //chyba, symbol jiz definovan
          //zahlasit semantickou chybu, nastavit kill, global error je-li potreba
          kill_after_analysis = true;
          if(global_error_code == SUCCESS) global_error_code = SEMANTIC_DEFINITION_ERROR;
          fprintf(stderr, "placeholder: redefinition of global id with key %s\n", get_cstring_string(key));
          return;
        }

        //nalezen key ale neni definovana
        curr->defined = true;
        curr->type = type;
        curr->number_of_params = param_count;
        return;
      }
      curr = curr->next;
    }
  }

  STItem *new = init_st_item();
  if(new == NULL) return;
  new->key = key;
  new->type = type;
  new->first_occur_line = line_count;
  if(type == ST_FUNCTION) new->number_of_params = param_count;
  new->defined = true;
  append_item(new);
}

void define_id_from_token(Token *token, int param_count)
{
  //TODO
  if(token == NULL) {
    fprintf(stderr, "[hojkas] symtable.c: define_id_from_token(): dostal empty token\n");
    return;
  }
  //TODO
  //butcher token, then call define_id_from_info
}

void add_undef_id_from_info(cstring *key)
{
  if(!in_global) {
    //potrebujeme prohledat i local
    if(search_st(local_st, key)) return; //už info o nem existuje, okay, end
  }
  if(search_st(global_st, key)) return;

  //neni v tabulce ani jedne
  STItem *new = init_st_item();
  if(new == NULL) return;
  new->key = key;
  new->first_occur_line = line_count;
  append_item(new);
}

void add_undef_id_from_token(Token *token)
{
  add_undef_id_from_info(token->str);
}

st_type get_id_type(Token *token)
{
  //TODO
  //get type of id in token
  return ST_UNDEFINED;
}

void clean_all_symtables()
{
  if(!in_global) destroy_symtable(&local_st);
  in_global = true;
  destroy_symtable(&global_st);
}

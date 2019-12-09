/** @symtable.c
 * Tabulka symbolů řešená pomocí hash table
 *
 * @author xstrna14
 * @version 1.0
 * @date 24.11.2019
 */

//TODO když to najde error, nečistí to key string... asi i jinde
//prostě přidat, že když to key nezpracuje, ať ho to vymaže
//TODO
//pridat check, kdyz to v non_define najde konfliktni typ?


#include "symtable.h"

STable* global_st = NULL;
STable* local_st = NULL;
bool in_global = true;
STItem* act_item = NULL;


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

  for(unsigned i = 0; i < size; i++) (*st)->item_array[i] = NULL;
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
  for(unsigned i = 0; i < size; i++) destroy_line(&((*st)->item_array[i]));
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
  new->bad_boy = false;
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
    if(compare_string(key, (char *) get_cstring_string(curr->key))) {
      found = true;
      act_item = curr;
      break;
    }
    curr = curr->next;
  }
  return found;
}

void create_item(cstring *key, st_type type, int param_count, bool defined)
{
  STItem *new = init_st_item();
  if(new == NULL) return;
  new->key = key;
  new->type = type;
  new->first_occur_line = line_count;
  new->bad_boy = false;
  if(type == ST_FUNCTION) new->number_of_params = param_count;
  else new->number_of_params = -1;
  new->defined = defined;
  append_item(new);
}

bool st_is_active()
{
  if(act_item == NULL) return false;
  else return true;
}

void set_st_act(STable *st)
{
  size_t size = st->size;
  for(unsigned i = 0; i < size; i++) {
    if(st->item_array[i] != NULL) {
      act_item = st->item_array[i];
      return;
    }
  }
  act_item = NULL;
}

void st_next(STable *st)
{
  if(act_item == NULL) return;
  if(act_item->next != NULL) {
    act_item = act_item->next;
    return;
  }
  unsigned line = hashCode(act_item->key) % st->size;
  line++;
  for(; line < st->size; line ++) {
    if(st->item_array[line] != NULL) {
      act_item = st->item_array[line];
      return;
    }
  }
  act_item = NULL;
}

bool is_act_defined()
{
  if(act_item->defined) return true;
  return false;
}

void deact_st()
{
  act_item = NULL;
}

//**************************
// PRÁCE SE SYMBOLY ZVNĚJŠÍ
//*************************

void start_symtable_with_functions()
{
  create_symtable(&global_st, GLOBAL_ST_SIZE);
  if(global_st == NULL) return; //error vyřešen ve volání create_symtable

  create_item(init_cstring("inputs"), ST_FUNCTION, 0, true);
  if(global_error_code == INTERNAL_ERROR) return;
  create_item(init_cstring("inputi"), ST_FUNCTION, 0, true);
  if(global_error_code == INTERNAL_ERROR) return;
  create_item(init_cstring("inputf"), ST_FUNCTION, 0, true);
  if(global_error_code == INTERNAL_ERROR) return;
  create_item(init_cstring("print"), ST_FUNCTION, -1, true);
  if(global_error_code == INTERNAL_ERROR) return;
  create_item(init_cstring("len"), ST_FUNCTION, 1, true);
  if(global_error_code == INTERNAL_ERROR) return;
  create_item(init_cstring("substr"), ST_FUNCTION, 3, true);
  if(global_error_code == INTERNAL_ERROR) return;
  create_item(init_cstring("ord"), ST_FUNCTION, 2, true);
  if(global_error_code == INTERNAL_ERROR) return;
  create_item(init_cstring("chr"), ST_FUNCTION, 1, true);
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
  if(in_global == true) return;
  in_global = true;
  local_check_def();
  destroy_symtable(&local_st);
}
/**
void define_id_from_info(cstring *key, st_type type, int param_count)
{
  //something that shouldn't but could go wrong?
  //search in local and global, if it is not already defined
  //if not, define it
  //if yes, error, print it, set stop after analysis, continue
  unsigned line;
  STItem *curr;

  if(!in_global) {
    //prohledat lokální i globální
    if(local_st == NULL) fprintf(stderr, "[hojkas] symtable.c: define_id_from_info(): Tabulka neexistuje\n");

    line = hashCode(key) % local_st->size;
    curr = local_st->item_array[line];

    while(curr != NULL) {
      if(compare_string(key, (char*) get_cstring_string(curr->key))) {
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
      if(compare_string(key, (char*) get_cstring_string(curr->key))) {
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

void define_id_from_token(Token *token, st_type type, int param_count)
{
  //TODO
  if(token == NULL) {
    fprintf(stderr, "[hojkas] symtable.c: define_id_from_token(): dostal empty token\n");
    return;
  }

  cstring *key = init_cstring(get_cstring_string(token->str)); //kopie token str
  add_undef_id_from_info(key, ST_UNDEFINED); //TODO fix with correct type

  define_id_from_info(key, type, param_count);
}*/

void add_undef_id_from_info(cstring *key, st_type type)
{
  //TODO ověř i typ!!!
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

void add_undef_id_from_token(Token *token, st_type type)
{
  cstring *key = init_cstring(get_cstring_string(token->str)); //kopie token str
  add_undef_id_from_info(key, ST_UNDEFINED); //TODO fix with correct type
}


//up there delete TODO just redirect flippz

st_type get_id_type(Token *token)
{
  bool found = false;
  if(!in_global) found = search_st(local_st, getTokenStrValue(token));
  if(!found) found = search_st(global_st, getTokenStrValue(token));
  if(!found || act_item == NULL) return ST_UNDEFINED; //nenalezeno
  return act_item->type;
}


bool work_out_fce_id(Token* token, int param_count, bool define)
{
  //nakopirovat ten token!
  deact_st();
  if(search_st(global_st, getTokenStrValue(token))) {
    //existuje tam uz zaznam s timto id
    if(act_item->type != ST_FUNCTION) {
      //error, spatny typ polozky, redefinice
      fprintf(stderr, "placeholder: work_out_fce_id: Snaha definovat %s, co bylo definovano jako id (SEMANTIC_DEFINITION_ERROR)\n", get_cstring_string(getTokenStrValue(token)));
      if(global_error_code == SUCCESS) global_error_code = SEMANTIC_DEFINITION_ERROR;
      return false;
    }
    if(define && act_item->defined == true) {
      //error, redefinice
      fprintf(stderr, "placeholder: work_out_fce_id: Snaha redefinovat funknci %s (SEMANTIC_DEFINITION_ERROR)\n", get_cstring_string(getTokenStrValue(token)));
      if(global_error_code == SUCCESS) global_error_code = SEMANTIC_DEFINITION_ERROR;
      return false;
    }
    if(act_item->number_of_params != param_count) {
      //spatny pocet parametru
      fprintf(stderr, "placeholder: work_out_fce_id: Nesedi pocet argumentu u %s (SEMANTIC_PARAMETER_ERROR)\n", get_cstring_string(getTokenStrValue(token)));
      if(global_error_code == SUCCESS) global_error_code = SEMANTIC_PARAMETER_ERROR;
      return false;
    }
    if(act_item->defined == false) {
      act_item->defined = define; //nechá false nebo definuje, podle "define"
      return true;
    }
    //jestli jsme tu, je zaroven mod false, takze vse sedi a proste se vracime
    return true;
  }
  else {
    //zaznam zatim neexistuje
    cstring* key = init_cstring(get_cstring_string(getTokenStrValue(token)));
    create_item(key, ST_FUNCTION, param_count, define);
    return true;
  }
}


bool work_out_val_id(Token *token, bool define)
{
  //nakopirovat ten token!
  deact_st();
  //prvne prohleda, jestli zaznam existuje, zaroven jestli neni funkce stejneho
  //jmena
  bool found_local = false;
  bool found_global = false;
  if(!in_global) found_local = search_st(local_st, getTokenStrValue(token));
  if(found_local == false) found_global = search_st(global_st, getTokenStrValue(token));

  //nyni je v found_local, found_global zda jsme ho nasli a act_item je pripadne
  //ten nalezeny
  if(!found_local && !found_global) {
    //prvek tam vubec neni
    if(define == false) {
      //chyba, pouziti nedefinovaneho id
      fprintf(stderr, "placeholder: work_out_val_id: Pouziti nedefinovaneho id %s (SEMANTIC_DEFINITION_ERROR)\n", get_cstring_string(getTokenStrValue(token)));
      if(global_error_code == SUCCESS) global_error_code = SEMANTIC_DEFINITION_ERROR;
      return false;
    }
    else {
      //id neni v seznamu ale chceme ho definovat
      cstring *key = init_cstring(get_cstring_string(getTokenStrValue(token)));
      create_item(key, ST_VALUE, 0, true);
      return true;
    }
  }

  if(act_item == NULL) {
    fprintf(stderr, "[hojkas] Tohle se stat nemelo... work_out_val_id\n");
    return false;
  }

  //prvek tam je, v act_item je odkaz na něj
  //chyba, pokud je to typ ST_FUNCTION
  if(act_item->type != ST_VALUE) {
    fprintf(stderr, "placeholder: work_out_val_id: Snaha pouzit %s jako id, ale bylo to uz pouzito jako funkce (SEMANTIC_DEFINITION_ERROR)\n", get_cstring_string(getTokenStrValue(token)));
    if(global_error_code == SUCCESS) global_error_code = SEMANTIC_DEFINITION_ERROR;
    return false;
  }

  //ze to doslo sem, znamena, ze je vse ok, neni treba redefinovat
  if(in_global) return true;
  //jsme v local, id je v local - vse ok, netreba nic delat
  if(found_local) return true;

  //dosli jsme sem - jsme v local, promenna je v global
  if(define == false) {
    //jsme v local, chceme pouzit globalni - to je ok, ale pokud by pozdeji
    //byla snaha definovat v local tu samou, je to chyba, proto
    //nastavim v global bad_boy flag
    act_item->bad_boy = true;
    return true;
  }
  else {
    //jsme v local, chceme definovat neco, co zastini globalni
    //to je ok, pokud u globalni (v act_item) neni nastaven bad_boy flag
    if(act_item->bad_boy == true) {
      //error
      fprintf(stderr, "placeholder: work_out_val_id: Definovani id %s, ktere by zastinilo globalni, ale uz bylo ve funkci pouzito jako globalni (SEMANTIC_DEFINITION_ERROR)\n", get_cstring_string(getTokenStrValue(token)));
      if(global_error_code == SUCCESS) global_error_code = SEMANTIC_DEFINITION_ERROR;
      return false;
    }

    //vse ok, vytvori polozku v loc
    cstring *key = init_cstring(get_cstring_string(getTokenStrValue(token)));
    create_item(key, ST_VALUE, 0, true);
    return true;
  }
}


void uni_check_def(STable *st)
{
  //TODO
  //provede semantickou kontrolu, aka projde symbol po symbolu
  //checkne, jestli je definovany, pokud ne, vypise chybu, nastavi
  //flag, kill_after_analysis, takhle vsechny

  set_st_act(st);
  while(st_is_active()) {
    if(!is_act_defined()) {
      printf("Polozka neni definovana\n");
    }
    st_next(st);
  }
}

void global_check_def()
{
  //uni_check_def(global_st);
}

void local_check_def()
{
  //uni_check_def(local_st);
}

void clean_all_symtables()
{
  if(!in_global) destroy_symtable(&local_st);
  in_global = true;
  destroy_symtable(&global_st);
}

void clean_bad_boys()
{
  set_st_act(global_st);
  while(st_is_active()) {
    if(!is_act_defined()) {
      printf("Polozka neni definovana\n");
    }
    st_next(st);
  }
}

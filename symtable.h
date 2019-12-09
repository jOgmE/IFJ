/** @symtable.h
 * Tabulka symbolů řešená pomocí hash table
 *
 * @author xstrna14
 * @version 1.0
 * @date 24.11.2019
 */

#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_

#include "errors.h"
#include "token.h"
#include "dynamic_string.h"
#include <stdbool.h>
#include <stdint.h> //potřeba pro hash funkci
#include "scanner.h" //kvůli line count

typedef enum
{
  ST_VALUE,
  ST_FUNCTION,
  ST_LABEL,
  ST_UNDEFINED
} st_type;

typedef struct STItem
{
  st_type type;            //Typ položky
  bool defined;            //True, byla-li už definovaná, jinak false
  bool bad_boy;            //flag na overeni, zda v local nedoslo k zastineni
                           //zastineni global promene az po jejim pouziti
  cstring *key;            //Identifikátor typu cstring (dynamický string)
  size_t first_occur_line; //Číslo řádku, na němž se poprvé objevila, pro
                           //případ, že by nastala chyba nedefinování
  int number_of_params;    //Je-li to funkce, je zde uveden počet parametrů,
                           //-1 znamená libovolný
  struct STItem *next;     //Ukazatel na další položku
} STItem;

typedef struct
{
  size_t size;
  STItem **item_array;
} STable;

//TODO keep here, usable from outside
void define_id_from_info(cstring *key, st_type type, int param_count);
void define_id_from_token(Token *token, st_type type, int param_count); //not yet
void go_in_global();
void go_in_local();
void start_symtable_with_functions();
void clean_all_symtables();
void add_undef_id_from_info(cstring *key, st_type type);
void add_undef_id_from_token(Token *token, st_type type);
st_type get_id_type(Token *token);

void global_check_def();
void local_check_def();

bool work_out_fce_id(Token*, int, bool); //done
bool work_out_val_id(Token*, bool); //started

//TODO delete vv from here
void create_item(cstring *key, st_type type, int param_count, bool defined);
void destroy_symtable(STable **st);
unsigned hashCode(cstring *key);
void create_symtable(STable **st, size_t size);
void append_item(STItem *item);
STItem *init_st_item();
bool search_st(STable *st, cstring *key);

bool st_is_active();
void set_st_act(STable *st);
void st_next(STable *st);
bool is_act_defined();
void deact_st();

#endif //_SYMTABLE_H_

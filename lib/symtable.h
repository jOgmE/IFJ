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
#include "../scanner/token.h"
#include "dynamic_string.h"
#include <stdbool.h>
#include <stdint.h> //potřeba pro hash funkci
//TODO delete this, should be probably linked in parser anyway
#include "../scanner/scanner.h"

typedef enum {
  ST_INT, ST_FLOAT, ST_STRING, ST_FUNCTION, ST_LABEL, ST_PARAM, ST_UNDEFINED
} st_type;

typedef struct STItem {
  st_type type;             //Typ položky
  bool defined;             //True, byla-li už definovaná, jinak false
  cstring *key;             //Identifikátor typu cstring (dynamický string)
  size_t first_occur_line;  //Číslo řádku, na němž se poprvé objevila, pro
                            //případ, že by nastala chyba nedefinování
  int number_of_params;     //Je-li to funkce, je zde uveden počet parametrů
  struct STItem *next;      //Ukazatel na další položku
} STItem;

typedef struct {
  size_t size;
  STItem **item_array;
} STable;

//TODO keep here, usable from outside
void define_id_from_info(cstring *key, st_type type, int param_count);
void define_id_from_token(Token *token, int param_count);
void go_in_global();
void go_in_local();
void start_symtable_with_functions();
void clean_all_symtables();
void add_undef_id_from_info(cstring *key);
void add_undef_id_from_token(Token *token);
st_type get_id_type(Token *token);

//TODO delete vv from here
void destroy_symtable(STable **st);
unsigned hashCode(cstring *key);
void create_symtable(STable **st, size_t size);
void append_item(STItem* item);
STItem* init_st_item();
bool search_st(STable *st, cstring *key);

#endif //_SYMTABLE_H_

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
//#include "scanner.h"

typedef enum {
  ST_INT, ST_FLOAT, ST_STRING, ST_FUNCTION, ST_LABEL, ST_UNDEFINED
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

void start_symtable_with_functions();
//TODO delete vv from here
void destroy_symtable(STable **st);
void append_item(STItem* item);
void define_id_from_info(cstring *id, st_type type, int param_count);

#endif //_SYMTABLE_H_

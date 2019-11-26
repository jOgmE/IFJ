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
//TODO delete this, should be probably linked in parser anyway
#include "scanner.h"

typedef enum {
  ST_INT, ST_FLOAT, ST_STRING, ST_FUNCTION, ST_LABEL, ST_UNDEFINED
} st_type;

typedef struct STItem {
  st_type type;         //Typ položky
  bool defined;         //True, byla-li už definovaná, jinak false
  cstring *id;          //Identifikátor typu cstring (dynamický string)
  int first_occur_line; //Číslo řádku, na němž se poprvé objevila, pro
                        //případ, že by nastala chyba nedefinování
  int number_of_params; //Je-li to funkce, je zde uveden počet parametrů
  STItem *next;         //Ukazatel na další položku
} STItem;

typedef struct STLine {
  STItem *first;
  STLine *next;
} STLine;

typedef struct {
  size_t size_lines;
  STLine *first_line;
} STable;

#endif //_SYMTABLE_H_

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

//nastavi tabulku na globalni, znici lokalni, vola se, kdyz
//se ukonci definovani funkce
void go_in_global();

//nastavi aktualni tabulku na lokalni, vytvori ji, vola se na zacatku
//definice funkce, aby jeji id byly na lokalni tabulku vytvareny
void go_in_local();

//vola se na zacatku parseru, vytvori globalni tabulku a nacpe do ni
//vestavene funkce (aby nebyla snaha je redefinovat)
void start_symtable_with_functions();

//vycisti globalni tabulku a lokalni, existuje-li
void clean_all_symtables();

//pozustatek predchoziho reseni, zavola work_out_val_id s parametrem false,
//je tu kvuli kompatibilitě se syntaktickou analyzou vyrazu
void add_undef_id_from_token(Token *token, st_type type);

//vrati typ id daneho tokenu (najde v tabulce polozku s danym klicem a vrati typ)
st_type get_id_type(Token *token);

//projede globalni tabulku, pokud najde funkci, co nebyla definovana,
//zahlasi chybu
void global_check_def();

//volano pri narazeni na ret, checkne, jestli nahodou nejsme v global,
//pokud jo, je to syntax error
bool is_this_ret_okay();

//zpracuje id funkce z tokenu, jeho parametry (int) a zda-li ho ma definovat
//nebo ne (bool), nalezite to zpracuje, pripadne vyhodi chybu
bool work_out_fce_id(Token*, int, bool);

//zpracuje id z tokenu a zda-li ho ma definovat
//nebo ne (bool), nalezite to zpracuje, pripadne vyhodi chybu
bool work_out_val_id(Token*, bool);

#endif //_SYMTABLE_H_

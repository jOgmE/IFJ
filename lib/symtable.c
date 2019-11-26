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
LOCAL_ST_SIZE = 20;
GLOBAL_ST_SIZE = 100

//*******************************
// TVORBA, ZMĚNA, ZÁNÍK TABULKY
//*******************************

void create_symtable(STable *st, size_t size)
{

}

void destroy_symtable(STable *st)
{

}

//**************************
// PRÁCE SE SYMBOLY ZVNĚJŠÍ
//*************************

void go_in_local()
{
  //TODO just placeholder, this should NOT happen
  if(in_global == false) fprintf(stderr, "[hojkas] symtable.c: go_in_local(): def fce uvnitr def fce, ma byt osetreno jinde\n");
  in_global = false;
  create_symtable(local_st, LOCAL_ST_SIZE);
}

void go_in_global()
{
  //TODO just placeholder, this should NOT happen
  if(in_global == true) fprintf(stderr, "[hojkas] symtable.c: go_in_global(): snaha vyskocit z funkce kdyz nejsme ve funkci\n");
  in_global = true;
  destroy_symtable(local_st);
}

void define_id_from_token(Token *token, st_type type, int param_count)
{
  //TODO
  if(token == NULL) fprintf(stderr, "[hojkas] symtable.c: define_id_from_token(): dostal empty token\n");

  
}

void define_id_from_info(cstring *id, st_type type, int param_count)
{

}

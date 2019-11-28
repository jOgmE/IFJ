/** @parser.c
 * Syntakticka analyza rekurzivnim sestupem vseho krom vyrazu
 *
 * @author xstrna14
 * @version 1.0
 * @date 27.11.2019
 */

 #include "parser.h"

//TODO delete, just for debug
 #include "../tests/hojkas/st_debug.c"

//MAIN PARSERU
 void parser_do_magic()
 {
   //inicializuje tabulku a nahraje base funkce
   start_symtable_with_functions();


   //TODO
   //prohledat tabulku, jestli v ni nezbylo neco nedef -> fce v symtable

   //ukonci tabulky
   clean_all_symtables();
 }

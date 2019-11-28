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

Token *curr_token = NULL;
Token *last_token = NULL;

//-------SIMULACNI FUNKCE, VYMAZAT-----------
//TODO delete

Token *fake_token()
{
  static int time = 0;
  Token *new = init_token();
  switch(time)
  {
    case 0:
      add_simple_data(new, EOFILE);
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    default:
      break;
  }

  time++;
  return new;
}

//------MAKRA---------------------
//vypíše chybové hlášení a nastaví global_error_code na error pokud už není
#define syntax_err(str, str2) char* norm = normalise_token_type(curr_token);\
fprintf(stderr, "Syntax error l. %4ld: %s %s %s", line_count, str, norm, str2);\
if(global_error_code == SUCCESS) global_error_code = SYNTAX_ANALYSIS_ERROR

//overi, jestli curr_token je typu type
#define Tis(type) getTokenType(curr_token) == type

//----------POMOCNE FCE---------------
bool flush_until(e_type token_type)
{
  if(Tis(token_type)) return true;
  while(curr_token != EOFILE) {
    curr_token = fake_token();
    if(Tis(token_type)) return true;
    if(Tis(EOFILE)) return false;
  }
}

//-----ROZKLADY-------------------
bool prog()
{
  //prog -> prog_body_with_def EOF
  bool okay = true;

  if(Tis(EOFILE) || Tis(DEF) || Tis(ID) || Tis(LPA) || Tis(IF) || Tis(PASS) || Tis(RETURN) || Tis(WHILE) || Tis(INT) || Tis(DEC)) {
    //prog -> prog_body_with_def EOF

  }
  else {
    //chyba, prisel spatny token
    syntax_err("Nevhodny token (", ") v danem kontextu. Timto nemuze zacinat zdrojovy soubor.\n");
    flush_until(EOFILE);
    //obvykle bych resila navratovou hodnotu, ale tady je eof, je to jedno, syntax done
  }

  return okay;
}

bool prog_body_with_def()
{
  //prog_body_with_def -> epsilon
  //prog_body_with_def -> command EOL more_EOL prog_body_with_def
  //prog_body_with_def -> def id ( param_list ) : EOL indent
  //                      non_empty_prog_body dedent prog_body_with_def
  bool okay = true;

  if()

  return okay;
}

//MAIN PARSERU
void parser_do_magic()
 {
   //inicializuje tabulku a nahraje base funkce
   start_symtable_with_functions();

   //nacte prvni token
   curr_token = fake_token();

   //prog <- pocatecni nonterminal, cely program
   prog();



   //TODO
   //prohledat tabulku, jestli v ni nezbylo neco nedef -> fce v symtable

   //ukonci tabulky
   clean_all_symtables();
 }

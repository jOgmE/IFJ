/** @parser.c
 * Syntakticka analyza rekurzivnim sestupem vseho krom vyrazu
 *
 * @author xstrna14
 * @version 1.0
 * @date 27.11.2019
 *
 * IMPORTANT: z nejakeho duvodu se stringy vypisuji v synt erroru jen par znaku, proverit TODO
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
      add_simple_data(new, ELSE);
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
#define syntax_err(str, str2) fprintf(stderr, "Syntax error l. %4ld: %s ", line_count, str);\
stderr_print_token_info();\
fprintf(stderr, " %s\n", str2);\
if(global_error_code == SUCCESS) global_error_code = SYNTAX_ANALYSIS_ERROR

/*overi, jestli curr_token je typu type*/
#define Tis(type) getTokenType(curr_token) == type

//----------POMOCNE FCE---------------
bool flush_until(e_type token_type)
{
  if(Tis(token_type)) return true;
  while(getTokenType(curr_token) != EOFILE) {
    curr_token = fake_token();
    if(Tis(token_type)) return true;
    if(Tis(EOFILE)) return false;
  }
  return false;
}

void stderr_print_token_info();

//-----ROZKLADY-------------------
bool prog();
bool non_empty_prog_body();
bool prog_body();
bool prog_body_with_def();
bool more_EOL();
bool command();
bool not_sure1();
bool not_sure2();
bool not_sure3();
bool param_list();
bool op();
bool item();
bool terminal(e_type type);
bool terminal_expr();

bool prog()
{
  //prog -> prog_body_with_def EOF
  bool okay = true;

  if(Tis(EOFILE) || Tis(DEF) || Tis(ID) || Tis(LPA) || Tis(IF) || Tis(PASS) || Tis(RETURN) || Tis(WHILE) || Tis(INT) || Tis(DEC)) {
    //prog -> prog_body_with_def EOF
    okay = prog_body_with_def();
    //insert heavy thing check
    okay = terminal(EOFILE);
  }
  else {
    //chyba, prisel spatny token
    syntax_err("Nevhodny token (", ") v danem kontextu. Timto nemuze zacinat zdrojovy soubor.\n");
    flush_until(EOFILE);
    //obvykle bych resila navratovou hodnotu, ale tady je eof, je to jedno, syntax done
    okay = false;
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

  //if()

  return okay;
}

bool terminal(e_type type)
{
  //zkontroluje jestli je tu terminal dobreho typu
  bool okay = true;
  if(type == getTokenType(curr_token)) curr_token = fake_token();
  else {
    //neni to terminal, ktery v tomto momente musime pro spravnou syntaxi dostat
    syntax_err("Token ", " je v dane konstrukci chybny (nebo je chyba v tom, co mu predchazelo).");
    fprintf(stderr, "Byl ocekavan token: ");
    if(type == INDENT) fprintf(stderr, "INDENT.\n");
    else if(type == EOFILE) fprintf(stderr, "EOFILE.\n");
    else if(type == EOL) fprintf(stderr, "EOL.\n");
    else if(type == DEF) fprintf(stderr, "DEF.\n");
    else if(type == LPA) fprintf(stderr, "(.\n");
    else if(type == RPA) fprintf(stderr, ").\n");
    else if(type == COL) fprintf(stderr, ":.\n");
    else if(type == COM) fprintf(stderr, ",.\n");
    else if(type == DEDENT) fprintf(stderr, "DEDENT.\n");
    else if(type == IF) fprintf(stderr, "IF.\n");
    else if(type == ELSE) fprintf(stderr, "ELSE.\n");
    else if(type == PASS) fprintf(stderr, "PASS.\n");
    else if(type == RETURN) fprintf(stderr, "RETURN.\n");
    else if(type == WHILE) fprintf(stderr, "WHILE.\n");
    else if(type == ASS) fprintf(stderr, "=.\n");
    else fprintf(stderr, "[hojkas] parser.c: terminal(): tato funkce zavolana na neco, co nema v chybe\n");
    okay = false;
  }
  return okay;
}

//------------------------------------------------------------------------------
//MAIN PARSERU
void parser_do_magic()
 {
   //inicializuje tabulku a nahraje base funkce
   start_symtable_with_functions();

   //nacte prvni token
   curr_token = fake_token();

   //prog <- pocatecni nonterminal, cely program
   printf("Is all okay? %d\n", prog());



   //TODO
   //prohledat tabulku, jestli v ni nezbylo neco nedef -> fce v symtable

   //ukonci tabulky
   clean_all_symtables();
 }

 //tu protoze mne stvalo jak pres to musim furt scrollovat, doslova zadny jiny duvod
 void stderr_print_token_info()
 {
     int i;
     double d;
     switch(getTokenType(curr_token))
     {
       case INDENT:
         fprintf(stderr, "INDENT");
         break;
       case DEDENT:
         fprintf(stderr, "DEDENT");
         break;
       case EOL:
         fprintf(stderr, "EOL");
         break;
       case DEC:
         getDecValue(curr_token, &d);
         fprintf(stderr, "DECIMAL: %f", d);
         break;
       case INT:
         getIntValue(curr_token, &i);
         fprintf(stderr, "INT: %d", i);
         break;
       case ID:
         fprintf(stderr, "ID: %s", get_cstring_string(getTokenStrValue(curr_token)));
         break;
       case STR:
         fprintf(stderr, "STRING: %s", get_cstring_string(getTokenStrValue(curr_token)));
         break;
       case L:
         fprintf(stderr, "<");
         break;
       case LEQ:
         fprintf(stderr, "<=");
         break;
       case G:
         fprintf(stderr, ">");
         break;
       case GEQ:
         fprintf(stderr, ">=");
         break;
       case EQ:
         fprintf(stderr, "==");
         break;
       case ASS:
         fprintf(stderr, "=");
         break;
       case NEQ:
         fprintf(stderr, "!=");
         break;
       case PLUS:
         fprintf(stderr, "+");
         break;
       case MINUS:
         fprintf(stderr, "-");
         break;
       case AST:
         fprintf(stderr, "*");
         break;
       case SL:
         fprintf(stderr, "/");
         break;
       case DSL:
         fprintf(stderr, "//");
         break;
       case COL:
         fprintf(stderr, ":");
         break;
       case LPA:
         fprintf(stderr, "(");
         break;
       case RPA:
         fprintf(stderr, ")");
         break;
       case EOFILE:
         fprintf(stderr, "EOFILE");
         break;
       case COM:
         fprintf(stderr, ",");
         break;
       case DEF:
         fprintf(stderr, "DEF");
         break;
       case ELSE:
         fprintf(stderr, "ELSE");
         break;
       case IF:
         fprintf(stderr, "IF");
         break;
       case NONE:
         fprintf(stderr, "NONE");
         break;
       case PASS:
         fprintf(stderr, "PASS");
         break;
       case RETURN:
         fprintf(stderr, "RETURN");
         break;
       case WHILE:
         fprintf(stderr, "WHILE");
         break;
       default:
         fprintf(stderr, "other (DOCS or TEMP_ID or some shit)");
         break;
     }
 }

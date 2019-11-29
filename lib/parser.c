/** @parser.c
 * Syntakticka analyza rekurzivnim sestupem vseho krom vyrazu
 *
 * @author xstrna14
 * @version 1.0
 * @date 27.11.2019
 *
 * IMPORTANT: z nejakeho duvodu se stringy vypisuji v synt erroru jen par znaku, proverit TODO
 * IMPORTANT: Vypisy s "neocekavana skladba" by to chtělo asi přepsat
 * IMPORTANT: není stav, že PBWD může začínat EOL... hodit do tabulky před to
 * MEOL a sem přihodit stavy podle tabulky
 * NOTE: zbytečne vícemásobné hlášení, souvisí s ne moc nice hlášeními
 * actually not that bad, vypise to postupne jak to vybublava, sice vickrat, ale vice infa...
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
  /*fprintf(stderr, "Loading state %d\n", time);*/
  switch(time)
  {
    case 0:
      add_simple_data(new, DEF);
      break;
    case 1:
      add_id(new, init_cstring("fce"));
      break;
    case 2:
      add_simple_data(new, LPA);
      break;
    case 3:
      add_simple_data(new, RPA);
      break;
    case 4:
      add_simple_data(new, COL);
      break;
    case 5:
      add_simple_data(new, EOL);
      break;
    case 6:
      add_simple_data(new, INDENT);
      break;
    case 7:
      add_simple_data(new, PASS);
      break;
    case 8:
      add_simple_data(new, EOL);
      break;
    case 9:
      add_simple_data(new, DEDENT);
      break;
    case 10:
      add_simple_data(new, PASS);
      break;
    case 11:
      add_simple_data(new, EOL);
      break;
    case 12:
      add_simple_data(new, EOL);
      break;
    case 13:
      add_simple_data(new, EOL);
      break;
    default:
      add_simple_data(new, EOFILE);
      break;
  }
  time++;
  return new;
}

//------MAKRA---------------------
//vypíše chybové hlášení a nastaví global_error_code na error pokud už není
#define syntax_err(str, str2) fprintf(stderr, "Syntax error l. %4ld: %s ", line_count, str);\
stderr_print_token_info();\
fprintf(stderr, " %s", str2);\
kill_after_analysis = true;\
if(global_error_code == SUCCESS) global_error_code = SYNTAX_ANALYSIS_ERROR

/*overi, jestli curr_token je typu type*/
#define Tis(type) getTokenType(curr_token) == type

//zkontroluje pritomnost fatalni chyby a pokud predchozi stav failnul,
//skoci na zotaveni
#define heavy_check(label) if(global_error_code == INTERNAL_ERROR) return false;\
if(can_continue != true) goto label

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

//-----ROZKLADY-------------------      // 6  / 14
bool prog();                            //done
bool non_empty_prog_body();             //done
bool prog_body();                       //done
bool prog_body_with_def();              //done
bool more_EOL();                        //done
bool command(); //started
bool not_sure1(); //
bool not_sure2(); //
bool not_sure3(); //
bool param_list(); //started
bool op(); //
bool item(); //
bool terminal(e_type type);             //done
bool terminal_expr(); //
bool work_out_id(); //started


bool prog() //---PROG---
{
  //prog -> prog_body_with_def EOF
  bool can_continue = true;

  if(Tis(EOFILE) || Tis(DEF) || Tis(ID) || Tis(LPA) || Tis(IF) || Tis(PASS) || Tis(RETURN) || Tis(WHILE) || Tis(INT) || Tis(DEC)) {
    //prog -> prog_body_with_def EOF
    can_continue = prog_body_with_def();
    heavy_check(prog_error);
    can_continue = terminal(EOFILE);
    heavy_check(prog_error);
    return true;
    //zotaveni se z chyby
    prog_error:
      flush_until(EOFILE); //tady mi to je jedno, ale jinde bych vracela toto
      return true; //true, ze vyssi muze pokracovat v checku
  }
  else {
    //chyba, prisel spatny token
    syntax_err("Nevhodny token (", ") v danem kontextu. Timto nemuze zacinat zdrojovy soubor.\n");
    flush_until(EOFILE);
    //obvykle bych resila navratovou hodnotu, ale tady je eof, je to jedno, syntax done
    return false;
  }
}



bool prog_body_with_def() //---PROG_BODY_WITH_DEF---
{
  //prog_body_with_def -> epsilon
  //prog_body_with_def -> command EOL more_EOL prog_body_with_def
  //prog_body_with_def -> def id ( param_list ) : EOL indent
  //                      non_empty_prog_body dedent prog_body_with_def
  bool can_continue = true;

  if(Tis(EOFILE)) {
    //prog_body_with_def -> epsilon
    //proste skonci uspesne
    return true;
  }
  else if(Tis(DEF)) {
    //prog_body_with_def -> def id ( param_list ) : EOL indent
    //                      non_empty_prog_body dedent prog_body_with_def

    //def
    can_continue = terminal(DEF);
    heavy_check(PBWD_r2e1);
    if(!kill_after_analysis) {
      //generate AC def start
      appendAC(DEF_START, NULL, NULL, NULL);
    }
    free_token(curr_token); //nepouzila jsem ho
    curr_token = fake_token();
    heavy_check(PBWD_r2e1); //co kdyby selhala alokace...

    //id
    can_continue = work_out_id();
    heavy_check(PBWD_r2e1);
    if(!kill_after_analysis) {
      //generate label of fce
      appendAC(LABEL, NULL, NULL, curr_token);
    }
    curr_token = fake_token(); //nepotrebuji free, token recyklovan do AC
    heavy_check(PBWD_r2e1);

    //(
    can_continue = terminal(LPA);
    heavy_check(PBWD_r2e1);
    free_token(curr_token);
    curr_token = fake_token();
    heavy_check(PBWD_r2e1);

    //param_list
    can_continue = param_list();
    heavy_check(PBWD_r2e1);

    //)
    can_continue = terminal(RPA);
    heavy_check(PBWD_r2e1);
    free_token(curr_token);
    curr_token = fake_token();
    heavy_check(PBWD_r2e1);

    //:
    can_continue = terminal(COL);
    heavy_check(PBWD_r2e1);
    free_token(curr_token);
    curr_token = fake_token();
    heavy_check(PBWD_r2e1);

    PBWD_r2rp1: //restore point 1
    //EOL
    can_continue = terminal(EOL);
    heavy_check(PBWD_r2e1);
    free_token(curr_token);
    curr_token = fake_token();
    heavy_check(PBWD_r2e1);

    //indent
    can_continue = terminal(INDENT);
    heavy_check(PBWD_r2e2);
    free_token(curr_token);
    curr_token = fake_token();
    heavy_check(PBWD_r2e2);

    //NEPB
    can_continue = non_empty_prog_body();
    heavy_check(PBWD_r2e2);

    //dedent
    can_continue = terminal(DEDENT);
    heavy_check(PBWD_r2e2);
    free_token(curr_token);
    curr_token = fake_token();
    heavy_check(PBWD_r2e2);

    //PBWD
    can_continue = prog_body_with_def();
    heavy_check(PBWD_r2e2);

    return true;

    //ERROR
    PBWD_r2e1:
      syntax_err("Nevhodny token (", ") v danem kontextu. Ocekavana skladba \"def id ( parametrs ) : EOL\".\n");
      if(flush_until(EOL) == false) return false;
      goto PBWD_r2rp1;
    PBWD_r2e2:
      syntax_err("Nevhodny token (", ") v danem kontextu. Ocekavana skladba \"indent program_body_with_definition dedent\".\n");
      can_continue = flush_until(DEDENT);
      return can_continue;
  }
  else if(Tis(ID) || Tis(LPA) || Tis(IF) || Tis(PASS) || Tis(RETURN) || Tis(WHILE) || Tis(INT) || Tis(DEC)) {
    //prog_body_with_def -> command EOL more_EOL prog_body_with_def
    //command
    can_continue = command();
    heavy_check(PBWD_r3e1);

    PBWD_r3rp1:
    //EOL
    can_continue = terminal(EOL);
    heavy_check(PBWD_r3e1);
    free_token(curr_token);
    curr_token = fake_token();
    heavy_check(PBWD_r3e1); //jen pro kontrolu inner erroru, takze navesti je jedno

    //more_EOL
    can_continue = more_EOL();
    heavy_check(PBWD_r3e2);

    //prog_body_with_def
    can_continue = prog_body_with_def();
    heavy_check(PBWD_r3e2);

    return true;

    //ERROR
    PBWD_r3e1:
      syntax_err("Nevhodny token (", ") v danem kontextu. Ocekavana skladba \"command EOL\".\n");
      if(flush_until(EOL) == false) return false; //neuspesny flush dojel na konec souboru
      goto PBWD_r3rp1;
    PBWD_r3e2:
      syntax_err("Nevhodny token (", ") v danem kontextu. Ocekavana skladba \"EOL more_EOL program_body_with_definitions\".\n");
      return false;
  }
  else {
    //sem by se to nemělo při dobré implementaci dostat
    fprintf(stderr, "[hojkas] parser.c: prog_body_with_def(): skoncilo v zakazanem stavu\n");
    return false;
  }
}



bool non_empty_prog_body() //---NON EMPTA PROGRAM BODY---
{
  //non_empty_prog_body -> more_EOL command EOL more_EOL PB
  bool can_continue = true;

  if(Tis(EOL) || Tis(ID) || Tis(LPA) || Tis(IF) || Tis(PASS) || Tis(RETURN) || Tis(WHILE) || Tis(INT) || Tis(DEC)) {
    //non_empty_prog_body -> more_EOL command EOL more_EOL PB
    //more_EOL
    can_continue = more_EOL();
    heavy_check(NEPB_r1e1);

    //command
    can_continue = command();
    heavy_check(NEPB_r1e1);

    NEPB_r1rp1:
    //EOL
    can_continue = terminal(EOL);
    heavy_check(NEPB_r1e1);
    free_token(curr_token);
    curr_token = fake_token();
    heavy_check(NEPB_r1e1);

    //more_EOL
    can_continue = more_EOL();
    heavy_check(NEPB_r1e2);

    //program_body
    can_continue = prog_body();
    heavy_check(NEPB_r1e2);

    return true;

    //ERROR
    NEPB_r1e1:
      syntax_err("Nevhodny token (", ") v danem kontextu. Ocekavana skladba \"more_EOL command EOL more_EOL program_body\".\n");
      if(flush_until(EOL) == false) return false;
      goto NEPB_r1rp1;
    NEPB_r1e2:
      syntax_err("Nevhodny token (", ") v danem kontextu. Ocekavana skladba \"more_EOL command EOL more_EOL program_body\".\n");
      return false;
  }
  else {
    //sem by se to nemělo při dobré implementaci dostat
    fprintf(stderr, "[hojkas] parser.c: non_empty_prog_body(): skoncilo v zakazanem stavu\n");
    return false;
  }
}



bool prog_body() //---PROG_BODY---
{
  //prog_body -> epsilon
  //prog_body -> command EOL more_EOL prog_body
  bool can_continue = true;

  if(Tis(ID) || Tis(LPA) || Tis(IF) || Tis(PASS) || Tis(RETURN) || Tis(WHILE) || Tis(INT) || Tis(DEC)) {
    //prog_body -> command EOL more_EOL prog_body
    //command
    can_continue = command();
    heavy_check(PB_r1e1);

    PB_r1rp1:
    //EOL
    can_continue = terminal(EOL);
    heavy_check(PB_r1e1);
    free_token(curr_token);
    curr_token = fake_token();
    heavy_check(PB_r1e1);

    //more_EOL
    can_continue = more_EOL();
    heavy_check(PB_r1e2);

    //program_body
    can_continue = prog_body();
    heavy_check(PB_r1e2);

    return true;

    //ERROR
    PB_r1e1:
      syntax_err("Nevhodny token (", ") v danem kontextu. Ocekavana skladba \"command EOL more_EOL program_body\".\n");
      if(flush_until(EOL) == false) return false;
      goto PB_r1rp1;
    PB_r1e2:
      syntax_err("Nevhodny token (", ") v danem kontextu. Ocekavana skladba \"command EOL more_EOL program_body\".\n");
      return false;
  }
  else if(Tis(DEDENT)) {
    //prog_body -> epsilon
    //proste jen skonci uspesne
    return true;
  }
  else {
    //sem by se to nemělo při dobré implementaci dostat
    fprintf(stderr, "[hojkas] parser.c: prog_body(): skoncilo v zakazanem stavu\n");
    return false;
  }
}



bool command() //---COMMAND---
{
  bool can_continue = true;
  if(Tis(PASS)) {
    //placeholder!!! kvůli testům
    free_token(curr_token);
    curr_token = fake_token();
  }
  return can_continue;
}



bool param_list() //---PARAM----
{
  bool can_continue = true;
  return can_continue;
}



bool more_EOL() //---MORE_EOL---
{
  //more_EOL -> epsilon
  //more_EOL -> EOL more_EOL
  bool can_continue = true;

  if(Tis(EOL)) {
    //more_EOL -> EOL more_EOL
    //EOL
    //neni treba overovat, ze mame EOL, jinak bychom sem nedosli
    free_token(curr_token);
    curr_token = fake_token();

    //more_EOL
    can_continue = more_EOL();
    heavy_check(MEOL_r1e1);

    return true;

    //error
    MEOL_r1e1:
      syntax_err("Nevhodny token (", ") v danem kontextu. Ocekavana skladba [nedosazitelne]\n");
      return false;
  }
  else if(Tis(EOFILE) || Tis(DEF) || Tis(ID) || Tis(LPA) || Tis(DEDENT) || Tis(IF) || Tis(PASS) || Tis(RETURN) || Tis(WHILE) || Tis(INT) || Tis(DEC)) {
    //more_eol -> epsilon
    return true;
  }
  else {
    //sem by se to nemělo při dobré implementaci dostat
    fprintf(stderr, "[hojkas] parser.c: more_EOL(): skoncilo v zakazanem stavu\n");
    return false;
  }
}


//funguje pro: ID
bool work_out_id()
{
  bool can_continue = true;
  return can_continue;
}



//funguje pro: INDENT DEDENT EOFILE EOL DEF ( ) : , IF ELSE PASS RETURN WHILE =
bool terminal(e_type type)
{
  //zkontroluje jestli je tu terminal dobreho typu
  bool can_continue = true;
  if(type == getTokenType(curr_token));
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
    can_continue = false;
  }
  return can_continue;
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
   //TODO delete this (but keep prog() calling)
   bool overall = prog();
   printf("\n_______________________________________\n");
   printf("Analysis complete?      ");
   if(overall) printf("YES\n");
   else printf("NO\n");
   printf("Analysis without error?");
   if(!kill_after_analysis) printf(" YES\n");
   else printf(" ERROR n. %d\n", global_error_code);
   printf("\n______________________________________\n");


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

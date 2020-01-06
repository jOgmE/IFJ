/** @parser.c
 * Syntakticka analyza rekurzivnim sestupem vseho krom vyrazu
 *
 * @author xstrna14
 * @version 3.0
 * @date 9.12.2019
 *
 * 2.0 - Syntax errors funguji
 * 3.0 - Semantika funguje
 *
 *
 another work:
      errory! jak je psat atd
 *
 *
 */

#include "parser.h"

Token *curr_token = NULL;
Token *last_token = NULL;
Token *id_to_assign = NULL;

int while_count = 0;
int if_count = 0;

//------MAKRA---------------------
//vypíše chybové hlášení a nastaví global_error_code na error pokud už není
#define syntax_err(str, str2) fprintf(stderr, "[ " ERROR_COLOR "ERROR" RESET_COLOR " ] Syntax error l. %4ld: %s ", line_count, str);\
stderr_print_token_info();\
fprintf(stderr, " %s " BOLD_WHITE "(SYNTAX_ANALYSIS_ERROR)" RESET_COLOR "\n", str2);\
kill_after_analysis = true;\
if(global_error_code == SUCCESS) global_error_code = SYNTAX_ANALYSIS_ERROR

/*overi, jestli curr_token je typu type*/
#define Tis(type) getTokenType(curr_token) == type
//vlozi soustavu podminek, ktera vraci true, je-li curr_token typu item
#define Tis_item (Tis(ID) || Tis(NONE) || Tis(INT) || Tis(DEC) || Tis(STR))
#define Tis_op (Tis(L) || Tis(LEQ) || Tis(G) || Tis(GEQ) || Tis(EQ) || Tis(NEQ) || Tis(PLUS) || Tis(MINUS) || Tis(AST) || Tis(SL) || Tis(DSL))


//zkontroluje pritomnost fatalni chyby a pokud predchozi stav failnul,
//skoci na zotaveni
#define heavy_check(label) if(global_error_code == INTERNAL_ERROR || global_error_code == LEXICAL_ANALYSIS_ERROR || curr_token == NULL) return false;\
if(can_continue != true) goto label

//----------POMOCNE FCE---------------
bool flush_until(e_type token_type)
{
  if(curr_token == NULL) return false;
  if(Tis(token_type)) return true;
  while(getTokenType(curr_token) != EOFILE) {
    curr_token = getToken();
    if(curr_token == NULL) return false;
    if(Tis(token_type)) return true;
    if(Tis(EOFILE)) return false;
  }
  return false;
}

typedef enum {
  L_WHILE_START, L_WHILE_LABEL, L_WHILE_END, L_IF_START, L_IF_ELSE, L_IF_END
} label_enum;

//vytvori label pro while nebo if, iteruje cislo, aby byl label unikatni
cstring* create_label(label_enum type, int number)
{
  cstring* str = NULL;
  char num[30];

  switch (type)
  {
    case L_WHILE_LABEL:
      sprintf(num, "l_wl_%d", number);
      str = init_cstring(num);
      break;
    case L_WHILE_START:
      sprintf(num, "l_ws_%d", number);
      str = init_cstring(num);
      break;
    case L_WHILE_END:
      sprintf(num, "l_we_%d", number);
      str = init_cstring(num);
      break;
    case L_IF_START:
      sprintf(num, "l_is_%d", number);
      str = init_cstring(num);
      break;
    case L_IF_ELSE:
      sprintf(num, "l_ie_%d", number);
      str = init_cstring(num);
      break;
    case L_IF_END:
      sprintf(num, "l_in_%d", number);
      str = init_cstring(num);
      break;
    default:
      fprintf(stderr, "[hojkas] Vytvareni labelu pro neexistujici hodnotu, interní chyba konstrukce programu.\n");
      if(global_error_code == SUCCESS) global_error_code = INTERNAL_ERROR;
      break;
  }
  return str;
}

void stderr_print_token_info();

//-----ROZKLADY-------------------          // 15 / 15
bool prog();                                //done
bool non_empty_prog_body();                 //done
bool prog_body();                           //done
bool prog_body_with_def();                  //done
bool more_EOL();                            //done
bool command();                             //done
bool not_sure1();                           //done
bool not_sure2();                           //done
bool not_sure3();                           //done
bool param_list(int*, bool);                //done
bool more_params(int*, bool);               //done
bool print_param_list();                    //done
bool print_more_params();                   //done
bool param_item(bool in_def);               //done
bool terminal(e_type type);                 //done


bool prog() //---PROG---
{
  //prog -> prog_body_with_def EOF
  bool can_continue = true;

  if(Tis(EOL) || Tis(EOFILE) || Tis(DEF) || Tis(STR) || Tis(ID) || Tis(LPA) || Tis(IF) || Tis(PASS) || Tis(RETURN) || Tis(WHILE) || Tis(INT) || Tis(DEC)) {
    //prog -> more_EOL prog_body_with_def EOF
    //more_EOL
    can_continue = more_EOL();
    heavy_check(prog_error);

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
    syntax_err("Nevhodny token (", ") v danem kontextu. Timto nemuze zacinat zdrojovy soubor.");
    flush_until(EOFILE);
    //obvykle bych resila navratovou hodnotu, ale tady je eof, je to jedno, syntax done
    return false;
  }
}



bool prog_body_with_def() //---PROG_BODY_WITH_DEF---
{
  //prog_body_with_def -> epsilon
  //prog_body_with_def -> command prog_body_with_def
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
    go_in_local(); //vytvori loc tabulku symbolu

    can_continue = terminal(DEF);
    heavy_check(PBWD_r2e1);
    if(!kill_after_analysis) {
      //generate AC def start
      appendAC(DEF_START, NULL, NULL, NULL);
    }
    free_token(curr_token); //nepouzila jsem ho
    curr_token = getToken();
    heavy_check(PBWD_r2e1); //co kdyby selhala alokace...

    //id
    if(!(Tis(ID))) {
      syntax_err("Token ", " je na tomto miste nespravny, ma zde byt id.");
      goto PBWD_r2e1;
    }
    Token *def_id = copy_token(curr_token); //ulozi id pro budouci overeni semantikou
    if(!kill_after_analysis) {
      //generate label of fce
      appendAC(LABEL, NULL, NULL, curr_token);
    }
    curr_token = getToken(); //nepotrebuji free, token recyklovan do AC
    heavy_check(PBWD_r2e1);

    //(
    can_continue = terminal(LPA);
    heavy_check(PBWD_r2e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(PBWD_r2e1);

    //param_list
    int param_count = 0;
    can_continue = param_list(&param_count, true);
    heavy_check(PBWD_r2e1_1);

    can_continue = work_out_fce_id(def_id, param_count, true); //will also define
    heavy_check(PBWD_r2e1);

    PBWD_r2rp1:
    //)
    can_continue = terminal(RPA);
    heavy_check(PBWD_r2e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(PBWD_r2e1);

    //:
    can_continue = terminal(COL);
    heavy_check(PBWD_r2e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(PBWD_r2e1);

    PBWD_r2rp2: //restore point 2
    //EOL
    can_continue = terminal(EOL);
    heavy_check(PBWD_r2e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(PBWD_r2e1);

    //indent
    can_continue = terminal(INDENT);
    heavy_check(PBWD_r2e2);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(PBWD_r2e2);

    //NEPB
    can_continue = non_empty_prog_body();
    heavy_check(PBWD_r2e2);

    //dedent
    can_continue = terminal(DEDENT);
    heavy_check(PBWD_r2e2);
    if(!kill_after_analysis) {
      //generate AC def end
      appendAC(DEF_END, NULL, NULL, NULL);
    }
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(PBWD_r2e2);

    go_in_global(); //vráti se zpět do globalní tabulky, lokalni checkne a zrusi

    //mEOL
    can_continue = more_EOL();
    heavy_check(PBWD_r2e2);

    //PBWD
    can_continue = prog_body_with_def();
    heavy_check(PBWD_r2e2);

    return true;

    //ERROR
    PBWD_r2e1_1:
      //chyba v parametrech
      if(flush_until(RPA) == false) return false;
      goto PBWD_r2rp1;
    PBWD_r2e1:
      if(flush_until(EOL) == false) return false;
      goto PBWD_r2rp2;
    PBWD_r2e2:
      go_in_global();
      can_continue = flush_until(DEDENT);
      return can_continue;
  }
  else if(Tis(ID) || Tis(LPA) || Tis(IF) || Tis(PASS) || Tis(RETURN) || Tis(WHILE) || Tis(STR) || Tis(INT) || Tis(DEC)) {
    //prog_body_with_def -> command prog_body_with_def
    //command
    can_continue = command();
    heavy_check(PBWD_r3e1);

    //prog_body_with_def
    can_continue = prog_body_with_def();
    heavy_check(PBWD_r3e1);

    return true;

    //ERROR
    PBWD_r3e1:
      return false;
  }
  else {
    syntax_err("Do pravidla program_body_with_def prisel nevhodny token ( ", " ).");
    return false;
  }
}



bool non_empty_prog_body() //---NON EMPTY PROGRAM BODY---
{
  //non_empty_prog_body -> more_EOL command PB
  bool can_continue = true;

  if(Tis(EOL) || Tis(ID) || Tis(STR) || Tis(LPA) || Tis(IF) || Tis(PASS) || Tis(RETURN) || Tis(WHILE) || Tis(INT) || Tis(DEC)) {
    //non_empty_prog_body -> more_EOL command PB
    //more_EOL
    can_continue = more_EOL();
    heavy_check(NEPB_r1e1);

    //command
    can_continue = command();
    heavy_check(NEPB_r1e1);

    //program_body
    can_continue = prog_body();
    heavy_check(NEPB_r1e1);

    return true;

    //ERROR
    NEPB_r1e1:
      return false;
  }
  else {
    syntax_err("Do pravidla non_empty_prog_body prisel nevhodny token ( ", " ).");
    return false;
  }
}



bool prog_body() //---PROG_BODY---
{
  //prog_body -> epsilon
  //prog_body -> command prog_body
  bool can_continue = true;

  if(Tis(ID) || Tis(STR) || Tis(LPA) || Tis(IF) || Tis(PASS) || Tis(RETURN) || Tis(WHILE) || Tis(INT) || Tis(DEC)) {
    //prog_body -> command prog_body
    //command
    can_continue = command();
    heavy_check(PB_r1e1);

    //program_body
    can_continue = prog_body();
    heavy_check(PB_r1e1);

    return true;

    //ERROR
    PB_r1e1:
      return false;
  }
  else if(Tis(DEDENT)) {
    //prog_body -> epsilon
    //proste jen skonci uspesne
    return true;
  }
  else {
    syntax_err("Do pravidla prog_body prisel nevhodny token ( ", " ).");
    return false;
  }
}



bool command() //---COMMAND---
{
  bool can_continue = true;
  if(Tis(PASS)) {
    //c -> pass EOL more_EOL
    //nemusím kontrolovat pass, jinak by to sem nedoslo
    free(curr_token);
    curr_token = getToken();
    heavy_check(C_r1e1);

    C_r1rp1:
    //EOL
    can_continue = terminal(EOL);
    heavy_check(C_r1e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r1e1); //jen pro kontrolu inner erroru, takze navesti je jedno

    //more_EOL
    can_continue = more_EOL();
    heavy_check(C_r1e2);

    return true;
    //error
    C_r1e1:
      if(flush_until(EOL) == false) return false;
      goto C_r1rp1;
    C_r1e2:
      return false;
  }
  else if(Tis(RETURN)) {
    //c -> return sure_expresion EOL more_EOL
    //nemusím kontrolovat return, jinak by to sem nedoslo
    free(curr_token);
    curr_token = getToken();

    can_continue = is_this_ret_okay();
    heavy_check(C_r2e1);

    Token *ret_id; //musi byt stejne jako u volani fce, dulezite
    if(!kill_after_analysis) {
      ret_id = init_token();
      heavy_check(C_r2e1);
      add_temp_id(ret_id, init_cstring("temp_ret"));
      heavy_check(C_r2e1);
    }

    if(Tis(NONE)) {
        if(!kill_after_analysis) {
          appendAC(RET, NULL, NULL, curr_token);
          heavy_check(C_r2e1);
        }
        else {
          free_token(curr_token);
        }
        curr_token = getToken();
        heavy_check(C_r2e1);
    }
    else {
      //zavola analyzu vyrazu
      Token *check = curr_token;
      curr_token = expressionAnalysis(curr_token, NULL, ret_id);
      heavy_check(C_r2e1);
      if(check == curr_token) {
        if(!kill_after_analysis) free_token(ret_id);
        ret_id = NULL; //nebyl tam expr, nic se nevraci
      }

      //vygenerovat ret_id navratovou hodnotu
      if(!kill_after_analysis) {
        appendAC(RET, NULL, NULL, ret_id);
        heavy_check(C_r2e1); //alok check, asi to nespadne na error label
      }
    }


    C_r2rp1:
    //EOL
    can_continue = terminal(EOL);
    heavy_check(C_r2e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r2e1); //jen pro kontrolu inner erroru, takze navesti je jedno

    //more_EOL
    can_continue = more_EOL();
    heavy_check(C_r2e2);

    return true;
    C_r2e1:
      if(flush_until(EOL) == false) return false;
      goto C_r2rp1;
    C_r2e2:
      return false;
  }
  else if(Tis(WHILE)) {
    //c-> while sure_expresion : EOL indent NEPB dedent
    //while
    int this_while_count = while_count;
    while_count++;

    can_continue = terminal(WHILE);
    heavy_check(C_r3e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r3e1);

    //L_WHILE_LABEL label
    //zpracovat cond
    //cond jump na L_WHILE_START
    //jump na L_WHILE_END
    //L_WHILE_START label
    //vlastni program
    //JUMP na WHILE_LABEL
    //L_WHILE_END label
    //while_count pridat

    //vytvori label while_label
    Token *label;
    if(!kill_after_analysis) {
      appendAC(WHILE_START, NULL, NULL, NULL);
      label = init_token();
      add_label(label, create_label(L_WHILE_LABEL, this_while_count));
      appendAC(LABEL, NULL, NULL, label);
    }

    //vytvori cond token pro vysledek SA
    Token *cond = NULL;
    if(!kill_after_analysis) {
      cond = init_token();
      add_temp_id(cond, init_cstring("temp_cond"));
      heavy_check(C_r3e1); //jen alloc_check
    }

    Token *check = curr_token;
    curr_token = expressionAnalysis(curr_token, NULL, cond);
    heavy_check(C_r3e1);
    if(curr_token == check) {
      syntax_err("Pred tokenem ", " nebyl expression ale byt mel.");
      goto C_r3e1;
    }
    //kill_after_analysis = true;
    //vytvori cond_jump na zaklade cond
    if(!kill_after_analysis) {
      label = init_token();
      add_label(label, create_label(L_WHILE_START, this_while_count));
      appendAC(COND_JUMP, cond, NULL, label);
      //jump na WHILE_END, sem se to dostane ve vysledem kodu, pokud
      //podminka cyklu jiz neplati
      label = init_token();
      add_label(label, create_label(L_WHILE_END, this_while_count));
      appendAC(JUMP, NULL, NULL, label);
      //vytvori label while starts, kam se skace po uspesne podmince
      label = init_token();
      add_label(label, create_label(L_WHILE_START, this_while_count));
      appendAC(LABEL, NULL, NULL, label);

      heavy_check(C_r3e1);
    }

    //:
    can_continue = terminal(COL);
    heavy_check(C_r3e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r3e1);

    C_r3rp1:
    //EOL
    can_continue = terminal(EOL);
    heavy_check(C_r3e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r3e1);

    //indent
    can_continue = terminal(INDENT);
    heavy_check(C_r3e2);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r3e2);

    //non_empty_prog_body
    can_continue = non_empty_prog_body();
    heavy_check(C_r3e2);

    C_r3rp2:
    //dedent
    can_continue = terminal(DEDENT);
    heavy_check(C_r3e2);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r3e2);

    if(!kill_after_analysis) {
      //vytvori jump na while_label
      label = init_token();
      add_label(label, create_label(L_WHILE_LABEL, this_while_count));
      appendAC(JUMP, NULL, NULL, label);
      //vytvori jump na while_end
      label = init_token();
      add_label(label, create_label(L_WHILE_END, this_while_count));
      appendAC(LABEL, NULL, NULL, label);
      //generuje while_end
      appendAC(WHILE_END, NULL, NULL, NULL);
      heavy_check(C_r3e2); //tohle nikdy neskoci, jen pro alloc check
    }

    //more_EOL
    can_continue = more_EOL();
    heavy_check(C_r3e2);

    return true;

    //error
    C_r3e1:
      if(flush_until(EOL) == false) return false;
      goto C_r3rp1;
    C_r3e2:
      if(flush_until(DEDENT) == false) return false;
      goto C_r3rp2;
  }
  else if(Tis(IF)) {
    //c -> if se : eol indent nepb dedent else : EOL indent nepb dedent
    //if don't need to check
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r4e1);

    //shrnuti:
    //vypocet cond
    //COND_JUMP na L_IF_START
    //JUMP na L_IF_ELSE
    //label L_IF_START
    //vlastni kod if casti
    //JUMP na L_IF_END
    //label L_IF_ELSE
    //vlastni kod else casti
    //label L_IF_END

    int this_if_count = if_count;
    if_count++;
    Token *label;
    //vytvori cond token pro vysledek SA
    Token *cond = NULL;
    if(!kill_after_analysis) {
      cond = init_token();
      heavy_check(C_r4e1);
      add_temp_id(cond, init_cstring("temp_cond"));
      heavy_check(C_r4e1); //jen alloc_check
    }

    Token *check = curr_token;
    curr_token = expressionAnalysis(curr_token, NULL, cond);
    heavy_check(C_r4e1);
    if(curr_token == check) {
      syntax_err("Pred tokenem ", " nebyl expression ale byt mel.");
      goto C_r4e1;
    }

    //:
    can_continue = terminal(COL);
    heavy_check(C_r4e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r4e1);

    C_r4rp1:
    //EOL
    can_continue = terminal(EOL);
    heavy_check(C_r4e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r4e1);

    //indent
    can_continue = terminal(INDENT);
    heavy_check(C_r4e2);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r4e2);

    if(!kill_after_analysis) {
      //cond_jump na L_IF_START
      label = init_token();
      add_label(label, create_label(L_IF_START, this_if_count));
      appendAC(COND_JUMP, cond, NULL, label);
      //JUMP na L_IF_ELSE
      label = init_token();
      add_label(label, create_label(L_IF_ELSE, this_if_count));
      appendAC(JUMP, NULL, NULL, label);
      //label L_IF_START
      label = init_token();
      add_label(label, create_label(L_IF_START, this_if_count));
      appendAC(LABEL, NULL, NULL, label);
      heavy_check(C_r4e2);
    }

    //non_empty_prog_body
    can_continue = non_empty_prog_body();
    heavy_check(C_r4e2);

    C_r4rp2:
    //dedent
    can_continue = terminal(DEDENT);
    heavy_check(C_r4e2);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r4e2);

    //else
    can_continue = terminal(ELSE);
    heavy_check(C_r4e3);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r4e3);

    //:
    can_continue = terminal(COL);
    heavy_check(C_r4e3);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r4e3);

    C_r4rp3:
    //EOL
    can_continue = terminal(EOL);
    heavy_check(C_r4e3);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r4e3);

    //indent
    can_continue = terminal(INDENT);
    heavy_check(C_r4e4);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r4e4);

    //JUMP na L_IF_END
    //label L_IF_ELSE
    if(!kill_after_analysis) {
      label = init_token();
      add_label(label, create_label(L_IF_END, this_if_count));
      appendAC(JUMP, NULL, NULL, label);

      label = init_token();
      add_label(label, create_label(L_IF_ELSE, this_if_count));
      appendAC(LABEL, NULL, NULL, label);
      heavy_check(C_r4e4);
    }

    //non_empty_prog_body
    can_continue = non_empty_prog_body();
    heavy_check(C_r4e2);

    //label L_IF_END
    if(!kill_after_analysis) {
      label = init_token();
      add_label(label, create_label(L_IF_END, this_if_count));
      appendAC(LABEL, NULL, NULL, label);
      heavy_check(C_r4e4);
    }

    C_r4rp4:
    //dedent
    can_continue = terminal(DEDENT);
    heavy_check(C_r4e4);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r4e4);

    //more_EOL
    can_continue = more_EOL();
    heavy_check(C_r4e4);

    return true;

    //error
    C_r4e1:
      if(flush_until(EOL) == false) return false;
      goto C_r4rp1;
    C_r4e2:
      if(flush_until(DEDENT) == false) return false;
      goto C_r4rp2;
    C_r4e3:
      if(flush_until(EOL) == false) return false;
      goto C_r4rp3;
    C_r4e4:
      if(flush_until(DEDENT) == false) return false;
      goto C_r4rp4;
  }
  else if(Tis(INT) || Tis(STR) || Tis(DEC) || Tis(LPA)) {
    //curr tokoen je int, str, dec, lpa, vse indikuje, ze je treba vyraz
    //poslat analzye EOL mEOL

    //jista analyza, just send it
    //dat dalsi parametr, aby nic negeneroval
    //vytvori cond token pro vysledek SA
    Token *ret_id = NULL;
    if(!kill_after_analysis) {
      ret_id = init_token();
      add_temp_id(ret_id, init_cstring("ret_id"));
      heavy_check(C_r5e1); //jen alloc_check
    }

    //neresim res, to by nemelo spadnout, nepotrebuji zadny 3ac
    curr_token = expressionAnalysis(curr_token, NULL, ret_id);
    heavy_check(C_r5e1);

    C_r5rp1:
    //EOL
    can_continue = terminal(EOL);
    heavy_check(C_r5e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r5e1); //jen pro kontrolu inner erroru, takze navesti je jedno

    //more_EOL
    can_continue = more_EOL();
    heavy_check(C_r5e2);

    return true;
    C_r5e1:
      if(flush_until(EOL) == false) return false;
      goto C_r5rp1;
    C_r5e2:
      return false;
  }
  else if(Tis(ID)) {
    //id not_sure1 eol meol
    //ulozim token s id, protoze nevim, jestli je to id funkce nebo normalni
    last_token = curr_token;
    curr_token = getToken();
    heavy_check(C_r6e1);

    can_continue = not_sure1();
    heavy_check(C_r6e1);

    last_token = NULL;
    id_to_assign = NULL;

    C_r6rp1:
    //EOL
    can_continue = terminal(EOL);
    heavy_check(C_r6e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(C_r6e1); //jen pro kontrolu inner erroru, takze navesti je jedno

    //more_EOL
    can_continue = more_EOL();
    heavy_check(C_r6e2);

    return true;
    C_r6e1:
      if(flush_until(EOL) == false) return false;
      goto C_r6rp1;
    C_r6e2:
      return false;
  }
  else {
    syntax_err("Do pravidla command prisel nevhodny token ( ", " ).");
    return false;
  }

}




bool param_list(int* param_count, bool in_def) //---PARAM_LIST----
{
  //param_list -> item more_params
  //param_list -> epsilon
  bool can_continue = true;

  if(Tis(INT) || Tis(DEC) || Tis(STR) || Tis(ID) || Tis(NONE)) {
    //param_list -> item more_params
    //item
    can_continue = param_item(in_def);
    heavy_check(PL_r1e1);
    (*param_count)++;

    can_continue = more_params(param_count, in_def);
    heavy_check(PL_r1e1);

    return true;

    //ERROR
    PL_r1e1:
      return false;
  }
  else if(Tis(RPA)) {
    //param_list -> epsilon
    //proste jen skonci uspesne
    return true;
  }
  else {
    syntax_err("Do pravidla param_list prisel nevhodny token ( ", " ).");
    return false;
  }
}



bool more_params(int* param_count, bool in_def) //---MORE_PARAMS---
{
  bool can_continue = true;
  //more_params -> epsilon
  //more_params -> , param_item more_params

  if(Tis(COM)) {
    //more_params -> , param_item more_params
    //,
    //neni treba overovat COM, jinak bychom sem nedosli
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(MP_r1e1);

    //item
    can_continue = param_item(in_def);
    heavy_check(MP_r1e1);
    (*param_count)++;

    //more_params
    can_continue = more_params(param_count, in_def);
    heavy_check(MP_r1e1);

    return true;

    //ERROR
    MP_r1e1:
      return false;
  }
  else if(Tis(RPA)) {
    //param_list -> epsilon
    //proste jen skonci uspesne
    return true;
  }
  else {
    syntax_err("Do pravidla more_params prisel nevhodny token ( ", " ).");
    return false;
  }
}



bool print_param_list() //---PRINT_PARAM_LIST----
{
  //param_list -> item
  //param_list -> epsilon
  bool can_continue = true;

  if(Tis(INT) || Tis(DEC) || Tis(STR) || Tis(ID) || Tis(NONE)) {
    //param_list -> item more_params
    //item
    can_continue = param_item(false); //print nikdy nemuze byt def
    heavy_check(PPL_r1e1);

    //vytvori volani za kazdym parametrem
    if(!kill_after_analysis) {
      Token *param = init_token();
      heavy_check(PPL_r1e1);
      add_id(param, init_cstring("print"));
      appendAC(CALL, NULL, NULL, param);
      heavy_check(PPL_r1e1);
    }

    can_continue = print_more_params();
    heavy_check(PPL_r1e1);

    return true;

    //ERROR
    PPL_r1e1:
      return false;
  }
  else if(Tis(RPA)) {
    //param_list -> epsilon
    //proste jen skonci uspesne
    return true;
  }
  else {
    syntax_err("Do pravidla print_param_list prisel nevhodny token ( ", " ).");
    return false;
  }
}



bool print_more_params() //---PRINT_MORE_PARAMS---
{
  bool can_continue = true;
  //more_params -> epsilon
  //more_params -> , param_item more_params

  if(Tis(COM)) {
    //more_params -> , param_item more_params
    //,
    //neni treba overovat COM, jinak bychom sem nedosli
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(PMP_r1e1);

    //item
    can_continue = param_item(false);
    heavy_check(PMP_r1e1);

    if(!kill_after_analysis) {
      Token *param = init_token();
      heavy_check(PMP_r1e1);
      add_id(param, init_cstring("print"));
      appendAC(CALL, NULL, NULL, param);
      heavy_check(PMP_r1e1);
    }

    //more_params
    can_continue = print_more_params();
    heavy_check(PMP_r1e1);

    return true;

    //ERROR
    PMP_r1e1:
      return false;
  }
  else if(Tis(RPA)) {
    //param_list -> epsilon
    //proste jen skonci uspesne
    return true;
  }
  else {
    syntax_err("Do pravidla print_more_params prisel nevhodny token ( ", " ).");;
    return false;
  }
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
    curr_token = getToken();
    heavy_check(MEOL_r1e1);

    //more_EOL
    can_continue = more_EOL();
    heavy_check(MEOL_r1e1);

    return true;

    //error
    MEOL_r1e1:
      syntax_err("Nevhodny token (", ") v danem kontextu.");
      return false;
  }
  else if(Tis(EOFILE) || Tis(DEF) || Tis(STR) || Tis(ID) || Tis(LPA) || Tis(DEDENT) || Tis(IF) || Tis(PASS) || Tis(RETURN) || Tis(WHILE) || Tis(INT) || Tis(DEC)) {
    //more_eol -> epsilon
    return true;
  }
  else {
    syntax_err("Do pravidla more_EOL prisel nevhodny token ( ", " ).");
    return false;
  }
}



bool not_sure1()
{
  bool can_continue = true;
  if(Tis(LPA)) {
    //id ( param list ), id je v last_token
    //(
    //nemusím checkovat LPA, jinak bych se sem nedostala
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(NS1_r1e1);

    int param_count = 0;
    //ulozi se, jestli jde o funkci print nebo ne
    bool is_print = false;

    if(is_print) {
      //pokud je id funkce print, resim jinak
      can_continue = print_param_list();
      heavy_check(NS1_r1e1);
    }
    else {
      //param_list obyč
      can_continue = param_list(&param_count, false);
      heavy_check(NS1_r1e1);
    }

    NS1_r1rp1:
    //)
    can_continue = terminal(RPA);
    heavy_check(NS1_r1e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(NS1_r1e1);

    if(is_print) {
      free_token(last_token);
    }
    else {
      //zpracovat id
      work_out_fce_id(last_token, param_count, false);
      if(!kill_after_analysis) {
        appendAC(CALL, NULL, NULL, last_token);
        heavy_check(NS1_r1e1);
      }
      else free_token(last_token);
    }

    return true;
    //errors
    NS1_r1e1:
      if(flush_until(RPA) == false) return false;
      goto NS1_r1rp1;
  }
  else if(Tis(ASS)) {
    //= not_sure2
    //nemusim checkovat ass
    free(curr_token);
    curr_token = getToken();
    heavy_check(NS1_r2e1);

    //ulozim last token do id to assing
    id_to_assign = last_token;
    last_token = NULL;

    Token* copy_id = copy_token(id_to_assign);
    work_out_val_id(copy_id, true); //zadefinuji id, kteremu se bude prirazovat

    can_continue = not_sure2();
    if(!can_continue || global_error_code != SUCCESS) return false;

    return true;
    NS1_r2e1:
      return false;
  }
  else if(Tis_op) {
    Token *ret_id;
    if(!kill_after_analysis) {
      ret_id = init_token();
      heavy_check(NS1_r3e1);
      add_temp_id(ret_id, init_cstring("ret_id"));
      heavy_check(NS1_r3e1);
    }
    curr_token = expressionAnalysis(last_token, curr_token, ret_id);
    heavy_check(NS1_r3e1);
    if(!kill_after_analysis) free_token(ret_id); //nikam vysledek neukladam

    return true;
    NS1_r3e1:
      //afaik nepotrebne, tam jde jen o aloc checky
      return false;
  }
  else if(Tis(EOL) || Tis(EOFILE)) {
    //epsilon
    work_out_val_id(last_token, false);
    return true;
  }
  else {
    //token, co nenalezi zadnemu moznemu pravidlu
    syntax_err("Do pravidla not_sure1 prisel nevhodny token ( ", " ).");
    return false;
  }
}



bool not_sure2()
{
  bool can_continue = true;
  if(Tis(ID)) {
    //id not_sure3
    last_token = curr_token;
    curr_token = getToken();
    heavy_check(NS2_r1e1);

    can_continue = not_sure3();
    heavy_check(NS2_r1e1);

    return true;

    NS2_r1e1:
      return false;
  }
  else if(Tis(INT) || Tis(DEC) || Tis(STR) || Tis(LPA)) {
    //int/float/str/( rest_expr
    //vytvori cond token pro vysledek SA
    Token *ret_id = NULL;
    if(!kill_after_analysis) {
      ret_id = init_token();
      add_temp_id(ret_id, init_cstring("ret_id"));
      heavy_check(NS2_r2e1); //jen alloc_check
    }

    curr_token = expressionAnalysis(curr_token, NULL, ret_id);
    heavy_check(NS2_r2e1);

    if(!kill_after_analysis) {
      appendAC(ASSIGN, ret_id, NULL, id_to_assign);
      heavy_check(NS2_r2e1);
    }
    id_to_assign = NULL;

    return true;
    NS2_r2e1:
      //nemelo by to sem jit, jen radsi
      return false;
  }
  else {
    //token, co nenalezi zadnemu moznemu pravidlu
    syntax_err("Do pravidla not_sure2 prisel nevhodny token ( ", " ).");
    return false;
  }
}



bool not_sure3()
{
  bool can_continue = true;
  if(Tis(LPA)) {
    // ( param_list )

    //(
    //nemusím checkovat LPA, jinak bych se sem nedostala
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(NS3_r1e1);

    int param_count = 0;
    bool is_print = false;

    if(is_print) {
      //funkce je print vip zachazeni
      can_continue = print_param_list();
      heavy_check(NS3_r1e1);
    }
    else {
      //param_list
      can_continue = param_list(&param_count, false);
      heavy_check(NS3_r1e1);
    }

    NS3_r1rp1:
    //)
    can_continue = terminal(RPA);
    heavy_check(NS3_r1e1);
    free_token(curr_token);
    curr_token = getToken();
    heavy_check(NS3_r1e1);

    if(is_print) {
      //tu jsme v "a = print()"
      free_token(last_token);
    }
    else {
      //zpracovat id
      work_out_fce_id(last_token, param_count, false);
      if(!kill_after_analysis) {
        appendAC(CALL, NULL, NULL, last_token);
        heavy_check(NS3_r1e2);
      }
      else free_token(last_token);
    }

    //nastvit ret value
    Token *ret_id; //stejne jmeno (cstring uvnitr) jako vzdy u return, dulezite
    if(!kill_after_analysis) {
      ret_id = init_token();
      heavy_check(NS3_r1e2);
      add_temp_id(ret_id, init_cstring("temp_ret"));
      heavy_check(NS3_r1e2);
    }

    if(!kill_after_analysis) {
      appendAC(ASSIGN, ret_id, NULL, id_to_assign);
    }
    else {
      free_token(id_to_assign);
    }


    id_to_assign = NULL;
    last_token = NULL;

    return true;

    NS3_r1e1:
      if(flush_until(RPA) == false) return false;
      goto NS3_r1rp1;
    NS3_r1e2:
      return false;
  }
  else if(Tis_op) {
    //op rest_expr

    Token *ret_id = NULL;
    if(!kill_after_analysis) {
      ret_id = init_token();
      add_temp_id(ret_id, init_cstring("ret_id"));
      heavy_check(NS3_r2e1); //jen alloc_check
    }

    curr_token = expressionAnalysis(last_token, curr_token, ret_id);
    heavy_check(NS3_r2e1);

    if(!kill_after_analysis) {
      appendAC(ASSIGN, ret_id, NULL, id_to_assign);
      heavy_check(NS3_r2e1);
    }
    id_to_assign = NULL;

    return true;
    NS3_r2e1:
      return false;
  }
  else if(Tis(EOL) || Tis(EOFILE)) {
    //epsilon
    //aka bylo to id1 = id2, id1 - id_to_assign, id2 - last_token
    can_continue = work_out_val_id(last_token, false);
    heavy_check(NS3_r3e1);

    if(!kill_after_analysis) {
      appendAC(ASSIGN, last_token, NULL, id_to_assign);
    }
    else {
      free_token(id_to_assign);
      free_token(last_token);
    }

    id_to_assign = NULL;
    last_token = NULL;

    return true;
    NS3_r3e1:
      return false;
  }
  else {
    //token, co nenalezi zadnemu moznemu pravidlu
    syntax_err("Do pravidla not_sure3 prisel nevhodny token ( ", " ).");
    return false;
  }
}



bool param_item(bool in_def)
{
  bool can_continue = true;
  if(Tis(INT) || Tis(DEC) || Tis(STR) || Tis(ID) || Tis(NONE)) {
    if(Tis(NONE) && in_def == true) {
      syntax_err("None (",") nemuze byt v definici funkce");
    }
    if(Tis(ID)) {
      work_out_val_id(curr_token, in_def);
    }
    if(!kill_after_analysis) {
      appendAC(PARAM, NULL, NULL, curr_token);
    }
    else {
      free_token(curr_token);
    }
    curr_token = getToken();
    heavy_check(PI_r1e1);
    return true;
    PI_r1e1:
      return false;
  }
  else {
    /*fprintf(stderr, "[hojkas] parser.c: param_item(): skoncilo v zakazanem stavu\n");*/
    syntax_err("Do pravidla param_item prisel nevhodny token ( ", " ).");
    return false;
  }
}



//funguje pro: INDENT DEDENT EOFILE EOL DEF ( ) : , IF ELSE PASS RETURN WHILE =
bool terminal(e_type type)
{
  //zkontroluje jestli je tu terminal dobreho typu
  bool can_continue = true;
  if(type == getTokenType(curr_token));
  else {
    //neni to terminal, ktery v tomto momente musime pro spravnou syntaxi dostat
    if(Tis(EOFILE) && type == EOL) return can_continue; //vip vyjimka
    //pro chybejici eol na konci
    syntax_err("Token ", " je v dane konstrukci chybny (nebo je chyba v tom, co mu predchazelo).");
    fprintf(stderr, " Byl ocekavan token: ");
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
   curr_token = getToken();
   if(global_error_code != SUCCESS) return;

   if(!prog()) kill_after_analysis = true;

   global_check_def();

   //ukonci tabulky
   clean_all_symtables();
 }

 //tu protoze mne stvalo jak pres to musim furt scrollovat,
 //doslova zadny jiny duvod
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
         fprintf(stderr, "other (DOCS or TEMP_ID or something else)");
         break;
     }
 }

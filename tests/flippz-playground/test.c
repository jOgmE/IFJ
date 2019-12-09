#include "../../token.h"
#include "../../dynamic_string.h"
#include "../../symtable.h"
#include "precAnalysis_lib.h"

Token *curr_token = NULL;
extern bool kill_after_analysis;

extern tAC* first_ac;
extern tAC* last_ac;
size_t line_count = 0;

//-------SIMULACNI FUNKCE-----------

e_type faking[100] = {
  //Sem vypises po rade typy tokenu, co chces simulovat (pokazde,
  //kdyz zavolas fake token, vytvori ti to token o tomto typu
  //a vrati ti ho, priste dalsi v rade, atd.)
  INT, PLUS, LPA, INT, PLUS, INT, RPA, EOL,
  EOFILE
};

//deklarace funkcí, co jsou dole, tyhle neřeš, jen používej
// -----NERES-----
Token *fake_token();
void fake_parser();
void print_type(ac_type type);
void print_all_ac_by_f();
void print_ac(tAC* ac);

#define Tis(type) getTokenType(curr_token) == type
#define Tis_item (Tis(ID) || Tis(NONE) || Tis(INT) || Tis(DEC) || Tis(STR))
#define Tis_op (Tis(L) || Tis(LEQ) || Tis(G) || Tis(GEQ) || Tis(EQ) || Tis(NEQ) || Tis(PLUS) || Tis(MINUS) || Tis(AST) || Tis(SL) || Tis(DSL))
//---NERES KONEC---

//delete
Token *fake_analysis(Token *op1, Token *op2, Token *res);
//#define expressionAnalysis(a, b, c) fake_analysis(a, b, c)

//MAIN
int main()
 {
   //inicializuje tabulku a nahraje base funkce
   start_symtable_with_functions();

   //nacte prvni token
   curr_token = fake_token();

   fake_parser();
   printf("______________________________________\n");
   print_all_ac_by_f(true);
   printf("_______________________________________\n");
   printf("Analysis without error?");
   if(!kill_after_analysis) printf(" YES\n");
   else printf(" ERROR n. %d\n", global_error_code);

   //ukonci tabulky
   clean_all_symtables();
 }



//-----NA TOHLE NESAHEJ, TO TE NEZAJIMA, PROSTE TO FUNGUJE-----

Token *fake_token()
{
  static int time = 0;
  Token *new = init_token();
  add_simple_data(new, faking[time]);
  new->str = init_cstring("temp_name");
  if(faking[time] != EOFILE) time++;
  //printf("loaded token\n");
  return new;
}

Token *fake_analysis(Token *op1, Token *op2, Token *res)
{
  while(Tis_item || Tis_op|| Tis(LPA) || Tis(RPA)) {
      //free_token(curr_token);
      curr_token = fake_token();
  }
  return curr_token;
}

void fake_parser()
{
  Token *ret_id = init_token();
  add_temp_id(ret_id, init_cstring("temp_ret"));
  while(1) {
      if(Tis(EOFILE)) break;
      if((Tis_item || Tis_op|| Tis(LPA) || Tis(RPA))) {
        appendAC(LABEL, NULL, NULL, NULL);
        curr_token = expressionAnalysis(curr_token, NULL, ret_id);
      }
      else curr_token = fake_token();
  }
}

void print_type(ac_type type)
{
  switch(type)
  {
    case LABEL: printf("LABEL"); break;
    case RET: printf("RET"); break;
    case CALL: printf("CALL"); break;
    case JUMP: printf("JUMP"); break;
    case COND_JUMP: printf("COND_JUMP"); break;
    case ADD: printf("ADD"); break;
    case SUB: printf("SUB"); break;
    case MUL: printf("MUL"); break;
    case DIV: printf("DIV"); break;
    case DIVINT: printf("DIVINT"); break;
    case ASSIGN: printf("ASSIGN"); break;
    case EQUAL: printf("EQUAL"); break;
    case NOT_EQUAL: printf("NOT_EQUAL"); break;
    case GREATER: printf("GREATER"); break;
    case GE: printf("GE"); break;
    case LESS: printf("LESS"); break;
    case LE: printf("LE"); break;
    case DEF_START: printf("DEF_START"); break;
    case DEF_END: printf("DEF_END"); break;
    case PARAM: printf("PARAM"); break;
    case WHILE_START: printf("WHILE_START"); break;
    case WHILE_END: printf("WHILE_END"); break;
    case UNDEFINED: printf("UNDEFINED"); break;
  }
}

void print_ac(tAC* ac) {
    printf("AC: ");
    print_type(ac->type);
    printf("\n");
    if(ac->op1 != NULL) printf("  1 %s\n", get_cstring_string(ac->op1->str));
    if(ac->op2 != NULL) printf("  2 %s\n", get_cstring_string(ac->op2->str));
    if(ac->res != NULL) printf("  r %s\n", get_cstring_string(ac->res->str));
}


void print_all_ac_by_f()
{
  setACAct();
  while(isACActive()) {
    print_ac(readAC());
    actAC();
  }
}

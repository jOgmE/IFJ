#include "../../dynamic_string.h"
#include "../../symtable.h"
#include "../../adress_code.h"

extern STable* global_st;
extern STable* local_st;

extern tAC* first_ac;
extern tAC* last_ac;

bool just_type = false;

//FILE *f = NULL;

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
  if(just_type) {
    printf("AC: ");
    print_type(ac->type);
    printf("\n");
    if(ac->op1 != NULL) printf("  1 %s\n", get_cstring_string(ac->op1->str));
    if(ac->op2 != NULL) printf("  2 %s\n", get_cstring_string(ac->op2->str));
    if(ac->res != NULL) printf("  r %s\n", get_cstring_string(ac->res->str));
    return;
  }
  printf("Adress code struct\n    type - %d\n", ac->type);
  printf("    Token op1 - %p\n", (void *) ac->op1);
  if(ac->op1 != NULL) {
    printf("          dbl - %f\n          int - %d\n          typ - %d\n", ac->op1->dec, ac->op1->i, ac->op1->type);
    if(ac->op1->str != NULL) {
      const char *str = get_cstring_string(ac->op1->str);
      printf("          str - %s\n", str);
    }
    else printf("          str - not\n");
  }
  printf("    Token op2 - %p\n", (void *) ac->op2);
  if(ac->op2 != NULL) {
    printf("          dbl - %f\n          int - %d\n          typ - %d\n", ac->op2->dec, ac->op2->i, ac->op2->type);
    if(ac->op2->str != NULL) {
      const char *str = get_cstring_string(ac->op2->str);
      printf("          str - %s\n", str);
    }
    else printf("          str - not\n");
  }
  printf("    Token op1 - %p\n", (void *) ac->res);
  if(ac->res != NULL) {
    printf("          dbl - %f\n          int - %d\n          typ - %d\n", ac->res->dec, ac->res->i, ac->res->type);
    if(ac->res->str != NULL) {
      const char *str = get_cstring_string(ac->res->str);
      printf("          str - %s\n", str);
    }
    else printf("          str - not\n");
  }
}

void print_all_ac()
{
  printf("--- print ac begin ---\n");
  tAC* curr = first_ac;
  while(curr != NULL) {
    print_ac(curr);
    curr = curr->next;
  }
  printf("--- print ac end ---\n");
}

void print_all_ac_by_f(bool mode)
{
  just_type = mode;
  setACAct();
  while(isACActive()) {
    print_ac(readAC());
    actAC();
  }
}

void print_item(STItem *i)
{
  printf("   I   %d -   %d - %3ld - %d - ", i->type, i->defined, i->first_occur_line, i->number_of_params);
  const char *str = get_cstring_string(i->key);
  printf("%s\n", str);
}

void print_line(STItem *i)
{
  printf("LINE: type -D/N- FOL -NOP- key\n");
  if(i == NULL) printf("empty\n");
  while(i != NULL) {
    print_item(i);
    i = i->next;
  }
}

void print_table(STable *st, size_t size)
{
  printf("\nTABLE BEGIN\n");
  for(int i = 0; i < size; i++) print_line(st->item_array[i]);

  printf("TABLE END\n\n");
}

void print_global()
{
  print_table(global_st, global_st->size);
}

void print_local()
{
  print_table(local_st, local_st->size);
}

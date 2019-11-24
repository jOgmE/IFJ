#include "adress_code.h"
#include "dynamic_string.h"

extern tAC* first_ac;
extern tAC* last_ac;

void print_ac(tAC* ac) {
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

void print_all_by_f()
{
  setACAct();
  while(isACActive()) {
    print_ac(readAC());
    actAC();
  }
}

int main()
{
  //print_all_ac();
  print_all_by_f();

  printf("Creating tokens and adding them, INT = 5, DEC = 5.5 and id = hi\n");
  Token* op1 = init_token();
  Token* op2 = init_token();
  Token* res = init_token();
  add_int(op1, 1);
  add_dec(op2, 1.0);
  cstring* str = init_cstring("jedna");
  add_id(res, str);
  appendAC(ADD, op1, op2, res);

  op1 = init_token();
  op2 = init_token();
  res = init_token();
  add_int(op1, 2);
  add_dec(op2, 2.0);
  cstring* str2 = init_cstring("dva");
  add_id(res, str2);
  appendAC(ADD, op1, op2, res);

  op1 = init_token();
  op2 = init_token();
  res = init_token();
  add_int(op1, 3);
  add_dec(op2, 3.0);
  cstring* str3 = init_cstring("tri");
  add_id(res, str3);
  appendAC(ADD, op1, op2, res);

  //print_all_ac();
  print_all_by_f();


  destroyACList();

  return 0;
}

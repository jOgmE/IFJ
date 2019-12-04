/** @adress_code.c
 * Funkce pro práce s tříadresným kódem
 *
 * @author xstrna14
 * @version 1.0
 * @date 23.11.2019
 */

#include "adress_code.h"

extern bool kill_after_analysis;

tAC *first_ac = NULL;
tAC *last_ac = NULL;
tAC *curr_ac = NULL;
tAC *breakpoint_ac = NULL;

tAC *initAC()
{
  tAC *ac = malloc(sizeof(tAC));
  if (ac == NULL)
  {
    print_internal_error(INTERNAL_ERROR, ERROR, "Interní chyba alokace paměti pro initAC v adress_code.c.\n");
    global_error_code = INTERNAL_ERROR;
    return NULL;
  }
  ac->type = UNDEFINED;
  ac->op1 = NULL;
  ac->op2 = NULL;
  ac->res = NULL;
  ac->next = NULL;
  return ac;
}

void appendAC(ac_type type, Token *op1, Token *op2, Token *res)
{
  tAC *ac = initAC();
  if (ac == NULL)
    return;
  ac->type = type;
  ac->op1 = op1;
  ac->op2 = op2;
  ac->res = res;
  if (first_ac == NULL)
  {
    first_ac = ac;
    last_ac = ac;
  }
  else
  {
    last_ac->next = ac;
    last_ac = ac;
  }
}

void destroyACList()
{
  tAC *curr = first_ac;
  tAC *next = curr;
  while (curr != NULL)
  {
    next = curr->next;
    if (curr->op1 != NULL)
      free_token(curr->op1);
    if (curr->op2 != NULL)
      free_token(curr->op2);
    if (curr->res != NULL)
      free_token(curr->res);
    free(curr);
    curr = next;
  }
}

void setACAct()
{
  curr_ac = first_ac;
}

void actAC()
{
  if (curr_ac != NULL)
    curr_ac = curr_ac->next;
}

bool isACActive()
{
  if (curr_ac == NULL)
    return false;
  return true;
}

void set_ac_breakpoint()
{
  //TODO odmazat, nemelo by k tomu nikdy dojít:
  if(breakpoint_ac != NULL) fprintf(stderr, "Breakpoint byl jiz nastaven, puvodni hodnota zahozena\n");
  if(curr_ac == NULL) fprintf(stderr, "Volani nad neaktualnim seznamem, nic se neprovede\n");
  breakpoint_ac = curr_ac;
}

void goto_ac_breakpoint()
{
  if(breakpoint_ac == NULL) fprintf(stderr, "Breakpoint nebyl nastaven, nedelam nic\n");
  else curr_ac = breakpoint_ac;
}

void changeLastRes(Token* token)
{
  //nakopiruje token a da ho místo posledního
  //prvku v posledním 3AC za podmínky,
  //že !kill_after_analysis
  if(last_ac == NULL || token == NULL || kill_after_analysis) return;
  if(last_ac->res != NULL) free(last_ac->res);
  Token *new = copy_token(token);
  last_ac->res = new;
}

tAC *readAC()
{
  return curr_ac;
}

ac_type readACtype()
{
  if (curr_ac != NULL)
    return curr_ac->type;
  return UNDEFINED;
}

Token *readACop1()
{
  if (curr_ac != NULL)
    return curr_ac->op1;
  return NULL;
}

Token *readACop2()
{
  if (curr_ac != NULL)
    return curr_ac->op2;
  return NULL;
}

Token *readACres()
{
  if (curr_ac != NULL)
    return curr_ac->res;
  return NULL;
}

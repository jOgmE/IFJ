/* \file token.c
 *
 * Implements functions working with the token.
 * Interface token
 *
 * \autor xpocsn00
 * \date 2019-11-03
 * \version 0.1
 */

#include "token.h"

//------global variables------
//
//struct converting keywords
convert keyword_conv[] = {
    {DEF, "def"},
    {ELSE, "else"},
    {IF, "if"},
    {NONE, "None"},
    {PASS, "pass"},
    {RETURN, "return"},
    {WHILE, "while"}
};
//struct converting operators
convert op_conv[] = {
    {L, "<"},
    {G, ">"},
    {ASS, "="},
    {PLUS, "+"},
    {MINUS, "-"},
    {AST, "*"},
    {SL, "/"},
    {DSL, "//"},
    {COL, ":"},
    {LPA, "("},
    {RPA, ")"},
    {COM, ","}
};


Token *init_token(){
    Token *tmp = (Token*)calloc(1, sizeof(Token)); //if error, this will be NULL
    if(!tmp){
        global_error_code = INTERNAL_ERROR;
        return NULL;
    }
    return tmp;
}

void add_simple_data(Token *token, e_type type){
    token->type = type;
    token->str = NULL;
    token->dec = 0;
    token->i = 0;
}

void add_int(Token *token, int num){
    add_simple_data(token, INT);
    token->i = num;
}

void add_dec(Token *token, double num){
    add_simple_data(token, DEC);
    token->dec = num;
}

void add_string(Token *token, cstring *str){
    add_simple_data(token, STR);
    token->str = str;
}

void add_id(Token *token, cstring *str){
    add_simple_data(token, ID);
    token->str = str;
}

void add_docs(Token *token, cstring *str){
    add_simple_data(token, DOCS);
    token->str = str;
}

void add_label(Token *token, cstring *str)
{
  if(token == NULL) return;
  add_simple_data(token, LAB);
  token->str = str;
}

void add_temp_id(Token *token, cstring *str)
{
  if(token == NULL) return;
  add_simple_data(token, TEMP_ID);
  token->str = str;
}

e_type getTokenType(Token *token){
    return token->type;
}

int getIntValue(Token *token, int *num){
    if(getTokenType(token) == INT){
        *num = token->i;
        return 0;
    }
    global_error_code = INTERNAL_ERROR;
    return -1;
}

int getDecValue(Token *token, double *num){
    if(getTokenType(token) == DEC){
        *num = token->dec;
        return 0;
    }
    global_error_code = INTERNAL_ERROR;
    return -1;
}

cstring *getTokenStrValue(Token *token){
    if(getTokenType(token) == STR || getTokenType(token) == ID){
        return token->str;
    }
    global_error_code = INTERNAL_ERROR;
    return NULL;
}

//used for testing
/*
Token **initTokenArr(size_t size){
    Token **token_arr;
    if((token_arr = (Token**)calloc(size, sizeof(Token*))) == NULL) return NULL;
    for(size_t i = 0; i<size; i++){
        if(!(token_arr[i] = init_token())){
            //if error happened
            for(size_t j = 0; j<=i; j++){
                //freeing the tokens initialized before
                free_token(token_arr[j]);
            }
            free(token_arr);
            return NULL;
        }
    }
    return token_arr;
}

int reallocTokenArr(Token **token_arr, size_t oldSize, size_t newSize){
    token_arr = (Token**)reallocarray(token_arr, newSize, sizeof(Token*));
    if(!token_arr){
        freeTokenArr(token_arr, oldSize);
        global_error_code = INTERNAL_ERROR;
        return -1;
    }

    for(size_t i=oldSize; i<newSize; i++){
        if(!(token_arr[i] = init_token())){
            //error happened
            for(size_t j=0; j<=i; j++){
                free_token(token_arr[j]);
            }
            free(token_arr);
            global_error_code = INTERNAL_ERROR;
            return -1;
        }
    }
    return 0;
}

void freeTokenArr(Token **token_arr, size_t size){
    for(size_t i=0; i<size; i++){
        if(!token_arr[i]) break;
        free_token(token_arr[i]);
    }
    free(token_arr);
}
*/

void free_token(Token *token){
    if(token->str){ //not NULL
        free_cstring(token->str);
    }
    free(token);
}

Token *copy_token(Token *token){
  Token *new = init_token();
  if(new == NULL){
    print_internal_error(INTERNAL_ERROR, ERROR, "Interní chyba alokace paměti pro token.\n");
    global_error_code = INTERNAL_ERROR;
    return NULL;
  }

  new->dec = token->dec;
  new->i = token->i;
  new->type = token->type;
  if(token->str == NULL) new->str == NULL;
  else {
    cstring *str = init_cstring(get_cstring_string(token->str));
    new->str = str;
  }
  return new;
}

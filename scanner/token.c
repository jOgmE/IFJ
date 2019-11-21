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

int init_token(Token *token){
    token = calloc(1, sizeof(Token));
    if(token == NULL) return -1;
    return 0;
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

e_type getTokenType(Token *token){
    return token->type;
}

int getIntValue(Token *token, int *num){
    if(getTokenType(token) == INT){
        *num = token->i;
        return 0;
    }
    return -1;
}

int getDecValue(Token *token, double *num){
    if(getTokenType(token) == DEC){
        *num = token->dec;
        return 0;
    }
    return -1;
}

int getStrValue(Token *token, char *str){
    (void)token;
    (void)str;
    return 0;
}

// written and directed by: Filip Draber
// Date: 27. 11. 2019

#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "symtable.h"
#include "errors.h"
#include "adress_code.h"
#include "PATable.h"

#ifndef _PA_STACK_H_
#define _PA_STACK_H_

typedef struct pastackelement
{
        char c;
        Token *content;
        struct pastackelement *belowPtr;
} PAStackElem;

typedef struct pastack
{
        PAStackElem *top;
} PAStack;

void PAInit(PAStack **s);

void PAPushFin(PAStack *s);

void PAPush(PAStack *s, Token *content);

PAStackElem *PATopTerm(PAStack *s);

int PAEndAtTop(PAStack *s);

Token *PATop(PAStack *s);

void PAAddBracket(PAStack *s);

int PAApplyRule(PAStack *s, Token *res, int *tempInt);

void PAPop(PAStack *s);

void PAYeet(PAStack *s);

void PAPrint(PAStack *s);

#endif

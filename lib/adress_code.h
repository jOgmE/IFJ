/** @adress_code.h
 * Funkce pro práce s tříadresným kódem
 *
 * @author xstrna14
 * @version 1.0
 * @date 23.11.2019
 */

#ifndef _ADRESS_CODE_H_
#define _ADRESS_CODE_H_

#include "../scanner/token.h"
#include "errors.h"

/**
* (FUNKCE, , , res) znamená, že první a druhý prostor pro ak není u funkce
* využit, v praxi bude NULL
* LABEL       - (LABEL, , , name) - vytvoří label s názvem "name"
* RET         - (RET, , , ) - vrátí se z funkce
*               (RET, , , res) - vrátí hodnotu
* CALL        - (CALL, , , label) - zavolá funkci na "label"
* JUMP        - (JUMP, , label) - skočí na "label"
* COND_JUMP   - (COND_JUMP, cond, , label) - je-li cond nenulová hodnota, skočí
*               na label, je-li 0, neskočí
* ASSIGN      - (ASSIGN, op1, , res) - přiřadí do res op1
* aritmetické funkce
* (PLUS, op1, op2, res)
* provede res = op1 + op2
*   PLUS,       - res = op1 + op2
*   MINUS,      - res = op1 - op2
*   TIMES,      - res = op1 * op2
*   DIVIDE,     - res = op1 / op2
*   DIVINT,     - res = op1 // op2
* relační funkce
* (EQUAL, op1, op2, res)
* platí-li (op1 == op2), nastaví res na nenulovou hodnotu, jinak na 0
*   EQUAL,      - op1 == op2
*   NOT_EQUAL,  - op1 != op2
*   GREATER,    - op1 > op2
*   GE,         - greater or equal, op1 >= op2
*   LESS,    - op1 < op2
*   LE          - less or equal, op1 <= op2
*/
typedef enum {
  LABEL, RET, CALL, JUMP, COND_JUMP,
  ACPLUS, ACMINUS, TIMES, DIVIDE, DIVINT, ASSIGN,
  EQUAL, NOT_EQUAL, GREATER, GE, SMALLER, SE
} ac_type;

typedef struct {
  ac_type type;
  Token op1;
  Token op2;
  Token res;
} adress_code;

void dosomething();



#endif //_ADRESS_CODE_H_

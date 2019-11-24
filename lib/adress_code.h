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
#include <stdbool.h>

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
* (ADD, op1, op2, res)
* provede res = op1 + op2
*   ADD,        - res = op1 + op2
*   SUB,        - res = op1 - op2
*   MUL,        - res = op1 * op2
*   DIV,        - res = op1 / op2
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
  ADD, SUB, MUL, DIV, DIVINT, ASSIGN,
  EQUAL, NOT_EQUAL, GREATER, GE, LESS, LE,
  UNDEFINED
} ac_type;

/*
* Pro funkce důležité tyto globální proměnné, potřebu-je li se na ně
* v jiných částech sahat, nutno vložit jako extern proměnné
* tAC* first_ac = NULL;
* tAC* last_ac = NULL;
*/

typedef struct tAC {
  ac_type type;
  Token* op1;
  Token* op2;
  Token* res;
  struct tAC *next;
} tAC;

//------------------------
// FCE pro parser
//------------------------

/**
* Inicializuje jeden řádek AC a vloží ho do listu.
* @param ac_type typ funkce AC
* @param *op1 ukazatel na token 1
* @param *op1 ukazatel na token 2
* @param *res ukazatel na třetí token, obvykle na uložení výsledku
* Není-li využit některý token, je třeba místo něj předat hodnotu NULL!!
*/
void appendAC(ac_type, Token* op1, Token* op2, Token* res);

/**
* Uvolní celý seznam AC včetně tokenů na něj navázaných.
*/
void destroyACList();

//----------------------
// FCE pro generátor
//----------------------

//Nastaví aktivitu na první prvek (nebo NULL, je-li seznam prázdný)
void setACAct();
//Posune aktivitu na další prvek, byl-li právě aktivní poslední nebo nebyl-li
//aktivní vůbec, aktivita není (NULL)
void actAC();
//Vrací "true" je-li aktivní prvek v seznamu, "false", není-li
bool isACActive();
//vrací ukazatel na aktivní položku seznamu
tAC* readAC();
//vrací typ aktivní položky seznamu nebo UNDEFINED nebyl-li seznam aktivní
ac_type readACtype();
//vrací op1 aktivní položky, NULL pokud daná položka pro příkaz neexistuje
//nebo seznam nebyl aktivní
Token* readACop1();
//jako readACop1, jen přečte op2
Token* readACop2();
//jako readACop1, jen přečte res
Token* readACres();


#endif //_ADRESS_CODE_H_

/* \file token.h
 *
 * This file contains the definition of the TOKEN.
 *
 * \autor xpocsn00
 * \date 2019-11-03
 * \version 0.1
 */

#ifndef _TOKEN_
#define _TOKEN_

/* INDENT - indent
 * DEDENT - dedent
 * EOL - end of line '\n'
 * DEC - decimal 0.0
 * INT - int 42
 * ID - identificator
 * STR - string
 * L - less '<'
 * LEQ - less than or equal '<='
 * G - greater '>'
 * GEQ - greater than or equal '>='
 * EQ - equal '=='
 * ASS - assign '='
 * NEQ - not equal '!='
 * PLUS - '+'
 * MINUS - '-'
 * AST - asterisk '*'
 * SL - slash '/'
 * DSL - double slash '//'
 * COL - colon ':'
 * LPA - left parentheses '('
 * RPA - right parentheses ')'
 */
typedef enum e_type{
    INDENT, DEDENT, EOL, DEC, INT, ID, STR,
    L, LEQ, G, GEQ, EQ, ASS, NEQ, PLUS, MINUS,
    AST, SL, DSL, COL, LPA, RPA
}

/* The structure represents one token.
 * type - holds the type of the token defined
 *        with e_type
 * dec, i, str holds the data
 *
 * when type id, str is being used to
 * store the pointer to the variable
 */
struct Token{
    double dec;
    int i;
    e_type type;
    char *str;
}Token;

#endif /*_TOKEN_*/

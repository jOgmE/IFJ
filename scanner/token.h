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
 * EOFILE - end of file
 * COM - comma ','
 */
typedef enum e_type{
    INDENT, DEDENT, EOL, DEC, INT, ID, STR,
    L, LEQ, G, GEQ, EQ, ASS, NEQ, PLUS, MINUS,
    AST, SL, DSL, COL, LPA, RPA, EOFILE, COM
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
    void *var_ptr;
    cstring *str;
}Token;

//---------------------------------------------------------
//             Functions to write a token
//---------------------------------------------------------

/* initializing the token
 * @param token to be initialized
 * @return 0 if no errors happened
 */
int init_token(Token *token);
/* Adding type to the token.
 *
 * This function is adding the token an e_type.
 * In case of complex token (with data f.e. DEC)
 * use the specific function (which is adding implicitly
 * the token type).
 *
 * @param token to set the type
 * @param e_type type of the token
 */
void add_simple_data(Token *token, e_type type);
/* Set token data (for int)
 *
 * Makes an INT type token with data.
 *
 * @param token to set
 * @param num data to be set
 */
void add_int(Token *token, int num);
/* Set token data (for decimal)
 *
 * Makes a DEC type token with data.
 *
 * @param token to set
 * @param num data to be set
 */
void add_dec(Token *token, double num);
/* Set token data (for string)
 *
 * Makes a STR type token with data.
 *
 * @param token to set
 * @param str data to be set
 */
void add_string(Token *token, cstring *str);

//---------------------------------------------------------
//                Functions to read a token
//---------------------------------------------------------

/* Returns the type of the token.
 */
e_type getTokenType(Token *token);
/* Returns the data for INT token.
 *
 * @param token reading from
 * @param num var the data to be saved
 * @returns !0 if the token type != INT otherwise 0
 */
int getIntValue(Token *token, int *num);
/* Returns the data for DEC token.
 *
 * @param token reading from
 * @param num var the data to be saved
 * @returns !0 if the token type != DEC otherwise 0
 */
int getDecValue(Token *token, double *num);
/* Returns the data for STR token.
 *
 * @param token reading from
 * @param num var the data to be saved
 * @returns !0 if the token type != STR otherwise 0
 */
int getStrValue(Token *token, char *str);

#endif /*_TOKEN_*/

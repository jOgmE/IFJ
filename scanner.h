/* @file scanner.h
 *
 * Header file for scanner.c
 *
 * @autor xpocsn00
 * @date 2019-11-02
 * @version 1.0
 *
 */

#ifndef _SCANNER_HEADER_FILE_
#define _SCANNER_HEADER_FILE_

#include <stdio.h>
#include <stdbool.h>
//#include <math.h>
#include "token.h"
#include "indent_stack.h"
#include "dynamic_string.h"
#include "errors.h"
#include "ifj19.h"

//starting state
#define S 1

//states Q
#define Q1 2
#define Q2 3
#define Q3 4
#define Q4 5
#define Q5 6
#define Q6 7
#define Q7 8
#define Q8 9
#define Q9 10
#define Q10 11
#define Q11 36
#define Q12 37
#define Q13 39

//Final states F
#define F1 12
#define F2 13 //there is no F3 for historical reasons
#define F4 15 //`erased state` :rtzW
#define F5 16
#define F6 17
#define F7 18
#define F8 19
#define F9 20
//#define F10 21
#define F11 22
#define F12 23
#define F13 24
#define F14 25
#define F15 26
#define F16 27
#define F17 28
//#define F18 29
#define F19 30
//#define F20 31
//#define F21 32
#define F22 33
#define F23 34
//#define F24 35
//#define F25 38
//the last number is 39 - Q13

//---------------------------------------------------------
//                    GLOBAL VARIABES
//---------------------------------------------------------

extern size_t line_count;
extern FILE *source_file;

/* Brief get a Token from the stream f
 *
 * Function implements the FSM needed to get a token.
 *
 * @param[in] File *f The file from where the characters are readed. Can be stdin
 * @param[out] Writing token to that pointer
 * @pre f != NULL
 * @pre f to be declared as global variabled
 * @pre isOpen(f) == true
 * @returns 0 on success otherwise errorcode
 */
Token *getToken();

//---------------------------------------------------------
//          Intern functions (don't use them)
//---------------------------------------------------------

/* Brief Checks if the input is white space.
 * @param[in] The character to be checked
 * @return true when ws otherwise false
 */
bool isSpace(char c);

/* Brief Checks if the input is an appropriate id name starting char.
 * @param[in] The character to be checked
 * @return true when char is appropriate
 */
bool isIdNameStart(char c);

/* Brief Checks if the input is an appropriate id name char.
 * @param[in] The character to be checked
 * @return true when char is appropriate
 */
bool isIdName(char c);

/* Brief Checks if the input is an appropriate number between 0-9 
 * @param[in] The character to be checked
 * @return true when char is appropriate
 */
bool is09num(char c);

/* Brief Checks if the input is an appropriate number between 1-9 
 * @param[in] The character to be checked
 * @return true when char is appropriate
 */
bool is19num(char c);

/* Brief Checks if the input is a printable char 
 * @param[in] The character to be checked
 * @return true when char is appropriate
 */
bool isPrintChar(char c);

/* Checks if the param string is keyword or not.
 *
 * @return on success returns e_type of the keyword otherwise 999
 */
e_type isKeyword(cstring *string);

/* Converts an cstring to integer,
 * makes an appropriate token of int and
 * frees the cstring
 *
 * @param our_string The string to be converted into an int
 * @param token The token to be writed
 * @returns 0 on success otherwise INTERNAL_ERROR
 */
int cstrToInt(cstring *our_string, Token *token);

/* Converts an cstring to decimal,
 * makes an appropriate token of dec and
 * frees the cstring
 *
 * @param our_string The string to be converted into a dec
 * @param token The token to be writed
 * @returns 0 on success otherwise INTERNAL_ERROR
 */
int cstrToDec(cstring *our_string, Token *token);

/* Checking if dedent needed or not
 *
 * If dedent needed it generates a DEDENT token
 * puts the last char back to the stream f
 * and pops the indent stack
 *
 * @param stack Stack for checking indents
 * @return True if dedent was DONE otherwise false
 */
bool checkDedent(tIndentStack *stack, char c, Token *token);

/* Checking if the char is a correct escape char
 *
 * @param c char to check
 * @return 1 if correct escape char
 * @return 2 if escape char with hexadecimal code
 * @return 0 when c is NO an escape char
 */
int isEscapeChar(char c);

/* Convering an escape char to a normal char
 *
 * @param c char to be converted
 * @return ascii char of the escape char
 */
char convertEscapeChar(char c);

/* Checking if the character is valid hexa value
 *
 * @param c char to check
 * @return true if valid otherwise false
 */
bool checkHexa(char c);

/* Converting hexadecimal value to a character
 *
 * @param s string of hexadecimal value
 * @return Char of the hexadecimal value
 */
char convertHexaToChar(char *s);

#endif /*_SCANNER_HEADER_FILE_*/

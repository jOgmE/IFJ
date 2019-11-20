/* @file scanner.h
 *
 * Header file for scanner.c
 *
 * @autor xpocsn00
 * @date 2019-11-02
 * @version 0.1
 *
 */

#ifndef _SCANNER_HEADER_FILE_
#define _SCANNER_HEADER_FILE_

#include <stdio.h>
#include <stdbool.h>
#include "token.h"
#include "../lib/indent_stack.h"

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

//Final states F
#define F1 12
#define F2 13 //there is no F3 for historical reasons
#define F4 15 //`erased state` :rtzW
#define F5 16
#define F6 17
#define F7 18
#define F8 19
#define F9 20
#define F10 21
#define F11 22
#define F12 23
#define F13 24
#define F14 25
#define F15 26
#define F16 27
#define F17 28
#define F18 29
#define F19 30
#define F20 31
#define F21 32
#define F22 33
#define F23 34
#define F24 35
//the last number is 37 - Q12

/* Brief get a Token from the stream f
 *
 * Function implements the FSM needed to get a token.
 *
 * \param[in] File *f The file from where the characters are readed. Can be stdin
 * \param[out] Writing token to that pointer
 * \pre f != NULL
 * \pre isOpen(f) == true
 * \returns 0 if ok.\n 2 if EOF
 */
int getToken(FILE *f, Token *token);

/* Brief Checks if the input is white space.
 * \param[in] The character to be checked
 * \return true when ws otherwise false
 */
bool isSpace(char c);

/* Brief Checks if the input is an appropriate id name starting char.
 * \param[in] The character to be checked
 * \return true when char is appropriate
 */
bool isIdNameStart(char c);

/* Brief Checks if the input is an appropriate id name char.
 * \param[in] The character to be checked
 * \return true when char is appropriate
 */
bool isIdName(char c);

/* Brief Checks if the input is an appropriate number between 0-9 
 * \param[in] The character to be checked
 * \return true when char is appropriate
 */
bool is09num(char c);

/* Brief Checks if the input is an appropriate number between 1-9 
 * \param[in] The character to be checked
 * \return true when char is appropriate
 */
bool is19num(char c);

/* Brief Checks if the input is a printable char 
 * \param[in] The character to be checked
 * \return true when char is appropriate
 */
bool isPrintChar(char c);

/*
 */
int checkInDedent(unsigned ws, tIndentStack *stack);
#endif /*_SCANNER_HEADER_FILE_*/

/*Autor: xpocsn00
 * date: 2019-11-02
 *
 * Header file for scanner.c
 */

#ifndef _SCANNER_HEADER_FILE_
#define _SCANNER_HEADER_FILE_

#include <stdio.h>

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

//Final states F
#define F1 12
#define F2 13
#define F3 14
#define F4 15
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

//defining Token
//SHOULD BE CHANGED
//USED ONLY TO GET NO ERROR!
typedef char *Token;

Token *getToken(FILE *f);

#endif /*_SCANNER_HEADER_FILE_*/

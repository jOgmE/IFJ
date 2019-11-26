/** @file generator.h
 *  Generátor výsledného kódu
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */
#ifndef _GENERATOR_H
#define _GENERATOR_H

#include "lib/dynamic_string.h"
#include "lib/errors.h"
#include "lib/adress_code.h"
#include "lib/gen_frame_tables.h"

#define INPUTS_FUNC
#define INPUTI_FUNC
#define INPUTF_FUNC
#define PRINT_FUNC
#define LEN_FUNC
#define SUBSTR_FUNC
#define ORD_FUNC
#define CHR_FUNC

extern frame_t CURRENT_FRAME;

/**
 * Incializace generátoru
 */
void init_gen();

/**
 * V případě chyby uzavře file stream odstraní výsledný soubor
 */
void remove_gen_file();

/**
 * Vytiskne všechen kód do výstupního souboru
 */
void print_gen_all();

/**
 * Uklizení prostředků použiých generátorem
 */
void free_gen();

//******************************************************************************************//
//******************************************************************************************//
//********************              Pomocné funkce generátoru                 **************//
//******************************************************************************************//
//******************************************************************************************//

/**
 * Změní současně používaný rámec a jeho string
 * 
 * @param frame Nový rámec
 */
void switch_frame(frame_t frame);

/**
 * Převede celé číslo na string pro zápis do souboru
 * 
 * @param data Číslo na převod
 * @returns Stringovou reprezentaci dat
 */
char *convert_int_to_string(int data);

/**
 * Převede realné číslo na string pro zápis do souboru
 * 
 * @param data Číslo na převod
 * @returns Stringovou reprezentaci dat
 */
char *convert_float_to_string(double data);

/**
 * Převede string na string kompatibilní s IFJcode19
 * 
 * @param string String na převod
 * @returns Stringovou reprezentaci dat
 */
char *convert_string(char *string);

/**
 * Vyhledá token v tabulce
 * 
 * @param token Token pro vyhledání
 * @returns Klíč tokenu v tabulce
 */
char *get_table_key_from_token(Token *token);

/**
 * Vyhledá token v tabulce
 * 
 * @param token Token pro vyhledání
 * @returns Token v tabulce
 */
Token *get_table_token(Token *token);

/**
 * Vyhledá typ tokenu v tabulce
 * 
 * @param token Token pro vyhledání
 * @returns Typ tokenu v tabulce
 */
e_type get_token_type(Token *token);

/**
 * Změní typ tokenu v tabulce
 * 
 * @param token Token pro změnu
 * @param e_type Nový typ tokenu
 */
void change_token_types(Token *token, e_type arithmetic_type);

/**
 * Změní porovna typy dvou tokenů symbolů a převede na stejný typ
 * 
 * @param op1 Symbol token pro změnu
 * @param op2 Symbol token pro změnu
 */
e_type compare_symbol_types_and_convert(Token *op1, Token *op2);

/**
 * Zapíše symbol proměnné
 * 
 * @param symb Token pro zápis
 * @param symb_frame Rámec tokenu symbolu
 * @param skip_space Přeskočení mezery za symbolem (pro zápis dalšich dat, např. $temp)
 */
void write_symbol_id(Token *symb, char *symb_frame, bool skip_space);

/**
 * Zapíše symbol hodnoty
 * 
 * @param symb Token pro zápis
 * @param skip_space Přeskočení mezery za symbolem (pro zápis dalšich dat, např. $temp)
 */
void write_symbol_type(Token *symb, bool skip_space);

/**
 * Zapíše symbol
 * 
 * @param symb Token pro zápis
 * @param symb_frame Rámec tokenu symbolu
 * @param skip_space Přeskočení mezery za symbolem (pro zápis dalšich dat, např. $temp)
 */
void write_symbol(Token *symb, char *symb_frame, bool skip_space);

/**
 * Zapíše přesun dat do proměnné
 * 
 * @param op1 Token pro přesun
 * @param op1_frame Rámec tokenu pro přesun
 * @param res Token pro výsledek přesunu
 * @param res_framec Rámec tokenu výsledku
 */
void write_move(Token *op1, char *op1_frame, Token *res, char *res_frame);

/**
 * Definuje proměnnou
 * 
 * @param res Název proměnné
 */
void write_defvar(Token *res);

/**
 * Zapíše label
 * 
 * @param label Název labelu
 */
void write_label(char *label);

/**
 * Zkontroluje, zda token byl už definován a případně ho definuje
 * 
 * @param token Token kontrolu a definici
 * @returns Rámec tokenu
 */
char *write_check_and_define(Token *token);

/**
 * Zpracuje převod typu proměnných
 * 
 * @param token Token na převod
 * @param frame_str Rámec tokenu
 * @param destType Typ na který se má převádět
 */
void write_convert_type(Token *token, char *frame_str, e_type destType);

/**
 * Zpracuje 3AK pro přiřazení
 * 
 * @param op1 Token pro přirazení do výslekdu
 * @param res Token pro výsledek
 */
void write_assign(Token *op1, Token *res);

/**
 * Zpracuje 3AK pro skok
 * 
 * @param res Token pro label na skok
 */
void write_jump(Token *res);

/**
 * Zpracuje 3AK pro pomíněný skok
 * 
 * @param op1 Token pro podmínku rovnou true
 * @param res Token pro label na skok
 */
void write_cond_jump(Token *op1, Token *res);

/**
 * Zpracuje 3AK pro porovnání
 * 
 * @param type Typ porovnání
 * @param op1 Token pro operand 1
 * @param op2 Token pro operand 2
 * @param res Token pro výsledek
 */
void write_comparison(ac_type type, Token *op1, Token *op2, Token *res);

/**
 * Zpracuje 3AK pro aritmetickou operaci
 * 
 * @param type Typ aritmerické operace
 * @param op1 Token pro operand 1
 * @param op2 Token pro operand 2
 * @param res Token pro výsledek
 */
void write_arithmetic(ac_type type, Token *op1, Token *op2, Token *res);

/**
 * Spustí generátor kódu nad seznamem 3AK
 */
void generate_code();

#endif // _GENERATOR_H
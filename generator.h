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

/**
 * Vloží instrukci na konec výsledného kódu
 */
void write_instruction();

/**
 * Vloží instrukce pro vytvoření funkce na konec výsledného kódu
 */
void create_function();

/**
 * Vloží instrukce pro volání funkce na konec výsledného kódu
 */
void call_function();

/**
 * Vloží instrukce pro návrat z funkce na konec výsledného kódu
 */
void return_from_function();

/**
 * Zpracuje 3AK pro přiřazení
 */
void write_assign(Token *op1, Token *res);

/**
 * Spustí generátor kódu nad seznamem 3AK
 */
void generate_code();

#endif // _GENERATOR_H
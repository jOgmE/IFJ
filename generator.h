/** @file generator.h
 *  Generátor výsledného kódu
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */
#ifndef GENERATOR_H
#define GENERATOR_H

#include "lib/dynamic_string.h"
#include "lib/errors.h"

/**
 * Incializace generátoru
 */
void init_gen();

/**
 * V případě chyby uzavře file stream odstraní výsledný soubor
 */
void remove_gen_file();

/**
 * Uklizení prostředků použiých generátorem
 */
void free_gen();

/**
 * Vloží instrukci na konec výsledného kódu
 */
void append_instruction();

#endif // GENERATOR_H
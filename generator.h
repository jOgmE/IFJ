/** @file generator.h
 *  Generátor výsledného kódu
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */
#ifndef GENERATOR_H
#define GENERATOR_H

#include "dynamic_string.h"
#include "errors.h"

/**
 * Incializace generátoru
 */
void init_gen();

/**
 * Uklizení prostředků použiých generátorem
 */
void free_gen();

/**
 * Vytiskne obsah výsledného řetězce do souboru zadaného parametrem
 * 
 * @param filename Název souboru, do kterého se bude tisknout
 */
void print_result_code(char *filename);

/**
 * Vloží instrukci na konec výsledného kódu
 */
void append_instruction(char *instruction);

#endif // GENERATOR_H
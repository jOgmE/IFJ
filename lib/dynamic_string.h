/** @file dynamic_string.h
 *  Knihovna pro řetězec s dynamickou délkou
 *  
 *  @author Jaroslav Hort (xhortj04)
 *  @date 2019
 */

#ifndef _DYNAMIC_STRING_H
#define _DYNAMIC_STRING_H

#include <stdlib.h>  //malloc, realloc, free
#include <string.h>  //strlen, strcmp. memcpy
#include <stdbool.h> //bool type
#include "errors.h"

// Velikost alokačního bloku
#define DYN_CHUNK_SIZE 256

typedef struct cstring_struct
{
    char *str;             // Ukazatel na alokovaný řetězec
    size_t length;         // Délka řetězce
    size_t allocated_size; // Alokovaná velikost pro řetězec
} cstring;

/**
 * Inicializuje novou instanci struktury cstring
 * 
 * @param str Ukazatel na řetězec, nebo řetez znaku pro inicializaci
 * @returns Ukazatel na alokovanou strukturu
 */

cstring *init_cstring(const char *str);

/**
 * Změní velikost struktury
 * 
 * @param str Ukazatel na strukturu
 * @param new_size Nová velikost sturktury
 */
void resize_cstring(cstring *cstr, size_t new_size);

/**
 * Vloží znak na konec řetězce, připadné rozšiří řetězec
 * 
 * @param cstr Ukazatel na strukturu
 * @param c Znak pro vložení
 */
void append_char(cstring *cstr, char c);

/**
 * Vloží řetězec na konec řetězce, připadné rozšiří řetězec
 * 
 * @param cstr Ukazatel na strukturu
 * @param str Ukazatel na řetězec pro vložení
 */
void append_string(cstring *cstr, char *str);

/**
 * Vloží rětězec ze struktury src za řetězec struktury dest
 * 
 * @param dest Ukazatel na cílovou strukturu
 * @param src Ukazatel na zdrojovou strukturu
 */
void append_cstring(cstring *dest, cstring *src);

/**
 * Porovná rětězce
 * 
 * @param cstr Ukazatel na strukturu na porovnání
 * @param str Ukazatel na řetězec pro porovnání
 * @returns TRUE pokud jsou stejné, jinak FALSE
 */
bool compare_string(cstring *cstr, const char *str);

/**
 * Vrátí ukazatel na rětězec uvnitř struktury
 * 
 * FUNKCE POUZE PRO ČTENÍ
 * NEUVOLŇUJTE VRÁCENÝ UKAZATEL
 *
 * @param cstr Ukazatel na strukturu
 * @returns Ukazatel na řetězec struktury, nebo NULL, pokud není alokován
 */
const char *get_cstring_string(cstring *cstr);

/**
 * Uvolní strukturu
 * 
 * @param cstr Ukazatel na strukturu
 */
void free_cstring(cstring *cstr);

#endif // _DYNAMIC_STRING_H

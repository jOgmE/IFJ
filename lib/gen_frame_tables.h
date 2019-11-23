/** @file gen_frame_tables.h
 *  Hash tabulky pro zapomatování proměnných v paměťových rámcích při generování kódu IFJCode19
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */

#ifndef _GEN_FRAME_TABLES_H
#define _GEN_FRAME_TABLES_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "errors.h"

#define extension_chunk 32

/**
 * Struktura položky tabulky
 */
typedef struct ftItem
{
    char *key;           //Klíč hodnoty
    struct ftItem *next; //Ukazatel na další hodnotu na stejném indexu
} ftItem_t;

// Struktura tabulky
typedef ftItem_t *fTable_t;

/**
 * Výčet typů tabulky
 */
typedef enum
{
    GLOBAL_FRAME, // Globalní rámec
    LOCAL_FRAME,  // Lokální rámec
    TEMP_FRAME    // Dočasný rámec
} frame_t;

// Definice proměnných pro globální tabulky
extern fTable_t *gf_table;
extern fTable_t *lf_table;
extern fTable_t *tf_table;

extern size_t GF_SIZE;
extern size_t LF_SIZE;
extern size_t TF_SIZE;

/**
 * Hash funkce pro získání klíče
 * 
 * @param key Název hodnoty pro vyhledání
 */
int hashcode(char *key, frame_t frame);

/**
 * Inicializace hash tabulky pro rámec
 * 
 * @param size Velikost tabulky
 * @param frame Rámec
 */
void init_table(size_t size, frame_t frame);

/**
 * Vyhledání položky v tabulkce
 * 
 * @param key Velikost tabulky
 * @param frame Rámec
 * @return Ukazatel na nalezenou položku
 */
ftItem_t *search_table(char *key, frame_t frame);

/**
 * Vložení položky do tabulky
 * 
 * @param item Velikost tabulky
 * @param frame Rámec
 */
void insert_table_item(char *item, frame_t frame);

/**
 * Vyhledání hodnoty v tabulce
 * 
 * @param key Velikost tabulky
 * @param frame Rámec
 * @return Hodnotu, pokud byla nalezena, NULL jinak
 */
char *get_table_item(char *key, frame_t frame);

/**
 * Zjištění, zda se položka nachází v tabulce
 * 
 * @param key Velikost tabulky
 * @param frame Rámec
 * @returns TRUE pokud položka existuje, FALSE pokud ne
 */
bool item_exists_table(char *key, frame_t frame);

/**
 * Odstraní položku z tabulky
 * 
 * @param key Velikost tabulky
 * @param frame Rámec
 */
void delete_table_item(char *key, frame_t frame);

/**
 * Vyčistí tabulku
 * 
 * @param frame Rámec
 */
void clear_table(frame_t frame);

#endif
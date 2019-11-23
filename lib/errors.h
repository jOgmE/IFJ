/** @file errors.h
 *  Knihovna pro práci s chybami
 *  
 *  @author Jaroslav Hort
 *  @date 2019
 */
#ifndef _ERRORS_H
#define _ERRORS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define INFO_COLOR "\033[36m"
#define WARNING_COLOR "\033[33m"
#define ERROR_COLOR "\033[31m"
#define BOLD_WHITE "\033[1m\033[37m"
#define RESET_COLOR "\033[0m"

/**
 * Výčet typů errorů
 */
typedef enum ErrorTypes
{
    SUCCESS,                            // Úspěch
    LEXICAL_ANALYSIS_ERROR = 1,         // Chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému, špatný počet mezer odsazení)
    SYNTAX_ANALYSIS_ERROR = 2,          // Chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu, neočekávaná změna úrovně odsazení)
    SEMANTIC_DEFINITION_ERROR = 3,      // Sémantická chyba v programu – nedefinovaná funkce/proměnná, pokus o redefinici funkce/proměnné, atp.
    SEMANTIC_RUNTIME_ERROR = 4,         // Sémantická/běhová chyba typové kompatibility v aritmetických, řetězcových a relačních výrazech
    SEMANTIC_PARAMETER_ERROR = 5,       // Sémantická chyba v programu – špatný počet parametrů u volání funkce
    SEMANTIC_OTHER_ERROR = 6,           // Ostatní sémantické chyby
    DIVISION_BY_ZERO_ERROR = 9,         // Běhová chyba dělení nulou
    INPUT_PARAMETER_ERROR = 50,         // Chybně zadané vstupní parametry na příkazovém řádku při spouštění interpretu
    INPUT_ANALYSIS_ERROR = 51,          // Chyba při analýze (lexikální, syntaktická) vstupního kódu v IFJcode19
    INPUT_SEMANTIC_ERROR = 52,          // Chyba při sémantických kontrolách vstupního kódu v IFJcode19
    INTERPRET_OPERAND_ERROR = 53,       // Běhová chyba interpretace – špatné typy operandů
    INTERPRET_VARIABLE_ERROR = 54,      // Běhová chyba interpretace – přístup k neexistující proměnné (rámec existuje)
    INTERPRET_SCOPE_ERROR = 55,         // Běhová chyba interpretace – rámec neexistuje (např. čtení z prázdného zásobníku rámců)
    INTERPRET_VALUE_ERROR = 56,         // Běhová chyba interpretace – chybějící hodnota (v proměnné, na datovém zásobníku, nebo v zásobníku volání)
    INTERPRET_OPERAND_VALUE_ERROR = 57, // Běhová chyba interpretace – špatná hodnota operandu (např. dělení nulou, špatná návratová hodnota instrukce EXIT)
    INTERPRET_STRING_ERROR = 58,        // Běhová chyba interpretace – chybná práce s řetězcem
    INTERPRET_INTERNAL_ERROR = 60,      // Interní chyba interpretu tj. neovlivněná vstupním programem (např. chyba alokace paměti, chyba při otvírání souboru s řídicím programem atd.)
    INTERNAL_ERROR = 99,                // Interní chyba překladače tj.neovlivněná vstupním programem(např.chyba alokace paměti, atd.)
} error_type;

// Řetězce pro výpis
extern char *error_type_names[10];
extern char *error_type_names50[11];

/**
 * error_severity
 */
typedef enum ErrorSeverity
{
    INFO,    // Informace
    WARNING, // Varování, kompilace pokračuje
    ERROR    // Chyba, kompilace selhala
} error_sev;

// Řetězce pro výpis
extern char *error_sev_names[3];

// Globální proměnná s kódem erroru
extern error_type global_error_code;

// Bool proměnná pro kontrolu zda nastala chyba při analýze
extern bool kill_after_analysis;

/**
 * Výpis ve formátu interní chyby
 * 
 * @param type Typ chyby
 * @param severity Závažnost chyby
 * @param message Zpráva pro vypsání
 */
void print_internal_error(error_type type, error_sev severity, char *message);

/**
 * Výpis ve formátu kompilační chyby
 * 
 * @param type Typ chyby
 * @param severity Závažnost chyby
 * @param line_num Číslo řádku ve zdrojové souboru, kde chyba nastala
 * @param file_name Název zdrovojého souboru
 * @param message Zpráva pro vypsání
 */
void print_compile_error(error_type type, error_sev severity, size_t line_num, char *file_name, char *message);

#endif // ERRORS_H
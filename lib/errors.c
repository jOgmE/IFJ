/** @file errors.c
 *  Implementace knihovny errorů
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */

#include "errors.h"

// Definice
error_type global_error_code = SUCCESS;

bool kill_after_analysis = false;

const char *error_type_names[] = {
    "SUCCESS",
    "LEXICAL_ANALYSIS_ERROR",
    "SYNTAX_ANALYSIS_ERROR",
    "SEMANTIC_DEFINITION_ERROR",
    "SEMANTIC_RUNTIME_ERROR",
    "SEMANTIC_PARAMETER_ERROR",
    "SEMANTIC_OTHER_ERROR",
    "",
    "",
    "DIVISION_BY_ZERO_ERROR",
};

const char *error_type_names50[] = {
    "INPUT_PARAMETER_ERROR",
    "INPUT_ANALYSIS_ERROR",
    "INPUT_SEMANTIC_ERROR",
    "INTERPRET_OPERAND_ERROR",
    "INTERPRET_VARIABLE_ERROR",
    "INTERPRET_SCOPE_ERROR",
    "INTERPRET_VALUE_ERROR",
    "INTERPRET_OPERAND_VALUE_ERROR",
    "INTERPRET_STRING_ERROR",
    "",
    "INTERPRET_INTERNAL_ERROR"};

const char *error_sev_names[] = {"INFO", "WARNING", "ERROR"};

char *get_error(error_type type)
{
    if (type >= 0 && type <= 9)
    {
        return error_type_names[type];
    }
    else if (type > 50)
    {
        return error_type_names50[type - 50];
    }
    else if (type == 99)
    {
        return "INTERNAL_ERROR";
    }
    else
    {
        return "";
    }
}

void print_internal_error(error_type type, error_sev severity, char *message)
{
    fprintf(stderr, "[%s] %s (%s)", get_error(type), error_sev_names[severity], message);
}

void print_compile_error(error_type type, error_sev severity, size_t line_num, char *file_name, char *message)
{
    fprintf(stderr, "[%s] %s (%s %s:%zu)", get_error(type), error_sev_names[severity], message, file_name, line_num);
}
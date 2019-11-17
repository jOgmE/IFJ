/** @file generator.h
 *  Implementace generátoru výsledného kódu
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */

#include "generator.h"

cstring *result_code = NULL;

void init_gen()
{
    if (result_code != NULL)
    {
        return;
    }

    result_code = init_cstring_size(1);

    if (result_code == NULL)
    {
        global_error_code = INTERNAL_ERROR;
        return;
    }
}

void free_gen()
{
    if (result_code == NULL)
    {
        return;
    }

    free_cstring(result_code);
}

void print_result_code(char *filename)
{
    if (result_code == NULL)
    {
        // TODO nastavit error?
        return;
    }

    FILE *result_file = fopen(filename, "w+");

    if (result_file == NULL)
    {
        global_error_code = INTERNAL_ERROR;
        return;
    }

    fputs(result_code->str, result_file);

    if (ferror(result_file) != 0)
    {
        global_error_code = INTERNAL_ERROR;
        return;
    }
}

void append_instruction(char *instruction)
{
    if (instruction == NULL)
    {
        return;
    }

    append_string(result_code, instruction);
}
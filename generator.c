/** @file generator.h
 *  Implementace generátoru výsledného kódu
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */

#include "generator.h"
#include "lib/gen_frame_tables.h"

FILE *result_code_file = NULL;
char *result_code_filename;

cstring *result_main_function_code = NULL;
cstring *result_functions_code = NULL;

void init_gen(char *filename)
{
    if (result_code_file != NULL)
    {
        return;
    }

    result_code_filename = strcat(filename, ".ic19");

    result_code_file = fopen(result_code_filename, "w");

    if (result_code_file == NULL)
    {
        global_error_code = INTERNAL_ERROR;
        perror("Chyba vytvoreni vystupniho souboru.");
        return;
    }

    fprintf(result_code_file, ".IFJcode19\nJUMP $$main\n");
    result_main_function_code = init_cstring("LABEL $$main\n");

    init_table(32, GLOBAL_FRAME);
    init_table(16, LOCAL_FRAME);
    init_table(16, TEMP_FRAME);
}

void remove_gen_file()
{
    if (result_code_file == NULL)
    {
        return;
    }

    if (remove(result_code_filename) != 0)
    {
        perror("Chyba odstranění výsledného souboru");
        global_error_code = INTERNAL_ERROR;
        return;
    }

    return;
}

void print_gen_all()
{
    if (result_code_file == NULL)
    {
        return;
    }

    if (result_functions_code != NULL)
    {
        fprintf(result_code_file, result_functions_code->str);
    }

    if (result_main_function_code != NULL)
    {
        fprintf(result_code_file, result_main_function_code->str);
    }
}

void free_gen()
{
    if (result_code_file == NULL)
    {
        return;
    }

    fclose(result_code_file);

    clear_table(GLOBAL_FRAME);
    clear_table(LOCAL_FRAME);
    clear_table(TEMP_FRAME);
}

void write_instruction()
{
    //TODO
}

void create_function()
{
    //TODO
}

void call_function()
{
    //TODO
}
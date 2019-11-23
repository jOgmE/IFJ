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

void append_instruction()
{
    //TODO
}
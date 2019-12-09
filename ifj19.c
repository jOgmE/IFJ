/** @file ifj19.c
 *  Hlavní rozhraní programu
 *
 *  @author Jaroslav Hort (xhortj04)
 *  @date 2019
 */

#include "ifj19.h"

FILE *source_file = NULL;

int main(int argc, char const *argv[])
{
    source_file = stdin;

    if (source_file == NULL)
    {
        perror("Vstupní soubor se nepodařilo otevřít.");
        return INTERNAL_ERROR;
    }

    //For errors.h
    //TODO get and set file name
    file_name = "input.py";

    parser_do_magic();

    if (global_error_code == SUCCESS)
    {
        init_gen();
        generate_code();
    }

    free_gen();

    return global_error_code;
}

/** @file ifj19.c
 *  Hlavní rozhraní programu
 *
 *  @author Jaroslav Hort (xhortj04)
 *  @date 2019
 */

#include "ifj19.h"

#define DEBUG

FILE *source_file = NULL;

int main(int argc, char const *argv[])
{
#ifdef DEBUG
    source_file = fopen(argv[1], "r");
    file_name = argv[1];
#else
    source_file = stdin;
    //For errors.h
    //TODO get and set file name
    file_name = "input.py";
#endif

    if (source_file == NULL)
    {
        perror("Vstupní soubor se nepodařilo otevřít.");
        return INTERNAL_ERROR;
    }

    parser_do_magic();

    if (global_error_code == SUCCESS)
    {
        init_gen();
        generate_code();
    }

    free_gen();

    return global_error_code;
}

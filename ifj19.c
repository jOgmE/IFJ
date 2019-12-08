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
    if (argc < 2)
    {
        perror("Nebyla zadana cesta ke vstupnimu souboru.");
    }

    source_file = fopen(argv[1], "r");

    if (source_file == NULL)
    {
        perror("Vstupní soubor se nepodařilo otevřít.");
        return 1;
    }

    //For errors.h
    file_name = argv[1];

    parser_do_magic();

    if (global_error_code == INTERNAL_ERROR)
    {
        fclose(source_file);
        return INTERNAL_ERROR;
    }

    if (global_error_code == SUCCESS)
    {
        init_gen();
        generate_code();
    }

    if (global_error_code == INTERNAL_ERROR)
    {
        fclose(source_file);
        return INTERNAL_ERROR;
    }

    free_gen();

    fclose(source_file);

    return global_error_code;
}

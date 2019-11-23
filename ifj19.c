#include <stdio.h>

#include "generator.h"

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        perror("Nebyla zadana cesta ke vstupnimu souboru.");
    }

    FILE *source = fopen(argv[1], "r");

    if (source == NULL)
    {
        perror("Vstupní soubor se nepodařilo otevřít.");
        return 1;
    }

    //insert scanner/parser call here

    init_gen();

    if (global_error_code == INTERNAL_ERROR)
    {
        remove_gen_file();
        return INTERNAL_ERROR;
    }

    free_gen();

    return 0;
}

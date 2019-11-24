/** @file generator.h
 *  Implementace generátoru výsledného kódu
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */

#include "generator.h"

FILE *result_code_file = NULL;
char *result_code_filename;

cstring *result_main_function_code = NULL;
cstring *result_functions_code = NULL;

frame_t CURRENT_FRAME = GLOBAL_FRAME;

void init_gen_test()
{
    result_main_function_code = init_cstring("LABEL $$main\n");
    result_functions_code = init_cstring_size(1);

    init_table(32, GLOBAL_FRAME);
    init_table(16, LOCAL_FRAME);
    init_table(16, TEMP_FRAME);
}

void init_gen(char *filename)
{
    if (result_code_file != NULL)
    {
        return;
    }

    cstring *file_name = init_cstring(filename);

    append_string(file_name, ".ic19");

    result_code_filename = file_name->str;

    result_code_file = fopen(result_code_filename, "w");

    if (result_code_file == NULL)
    {
        global_error_code = INTERNAL_ERROR;
        print_internal_error(INTERNAL_ERROR, ERROR, "Chyba vytvoreni vystupniho souboru");
        return;
    }

    fprintf(result_code_file, ".IFJcode19\nJUMP $$main\n");
    result_main_function_code = init_cstring("LABEL $$main\n");
    result_functions_code = init_cstring_size(1);

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
        print_internal_error(INTERNAL_ERROR, ERROR, "Chyba odstranění výsledného souboru");
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
        fprintf(result_code_file, "%s", result_functions_code->str);
    }

    if (result_main_function_code != NULL)
    {
        fprintf(result_code_file, "%s", result_main_function_code->str);
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

void write_move(cstring *code_block, char *frame_string, Token *op1, Token *res)
{
    append_string(code_block, "MOVE ");

    //var
    append_string(code_block, frame_string);
    append_string(code_block, "@");
    append_string(code_block, res->str->str);
    append_string(code_block, " ");

    //symb

    switch (op1->type)
    {
    case ID:
        append_string(code_block, frame_string);
        break;
    case INT:
        append_string(code_block, "int");
        break;
    case DEC:
        append_string(code_block, "float");
        break;
    case STR:
        append_string(code_block, "string");
        break;
    default:
        break;
    }

    append_string(code_block, "@");
    append_string(code_block, op1->str->str);

    append_string(code_block, "\n");
}

void write_defvar(cstring *code_block, char *frame_string, Token *res)
{
    append_string(code_block, "DEFVAR ");

    //var
    append_string(code_block, frame_string);
    append_string(code_block, "@");
    append_string(code_block, res->str->str);

    append_string(code_block, "\n");
}

void write_assign(Token *op1, Token *res)
{
    bool res_exists_global = item_exists_table(res->str->str, GLOBAL_FRAME);
    bool res_exists_current = item_exists_table(res->str->str, CURRENT_FRAME);

    cstring *code_block = CURRENT_FRAME == GLOBAL_FRAME ? result_main_function_code : result_functions_code;

    char *frame_string = get_frame_string(CURRENT_FRAME);

    if (!res_exists_current)
    {
        insert_table_item(res->str->str, CURRENT_FRAME);

        write_defvar(code_block, frame_string, res);
    }

    write_move(code_block, frame_string, op1, res);
}

void generate_code()
{
    setACAct();

    ac_type type;
    Token *op1 = NULL, *op2 = NULL, *res = NULL;

    while (isACActive())
    {
        type = readACtype();

        op1 = readACop1();
        op2 = readACop2();
        res = readACres();

        switch (type)
        {
        case ASSIGN:
            write_assign(op1, res);
            break;

        default:
            break;
        }

        actAC();
    }
}

int main(int argc, char const *argv[])
{
    init_gen("testout");

    Token *op1 = malloc(sizeof(Token));
    Token *res = malloc(sizeof(Token));

    cstring *op1_str = init_cstring("50");
    cstring *res_str = init_cstring("result");

    op1->str = op1_str;
    op1->type = INT;
    res->str = res_str;

    write_assign(op1, res);

    print_gen_all();

    free_gen();

    return 0;
}

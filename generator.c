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

cstring *CURRENT_BLOCK;
frame_t CURRENT_FRAME = GLOBAL_FRAME;
char *CURRENT_FRAME_STRING;

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

    fprintf(result_code_file, ".IFJcode19\nJUMP $$main\n\n");
    result_main_function_code = init_cstring("LABEL $$main\n");
    result_functions_code = init_cstring_size(1);

    CURRENT_FRAME = GLOBAL_FRAME;
    CURRENT_BLOCK = result_main_function_code;
    CURRENT_FRAME_STRING = get_frame_string(CURRENT_FRAME);

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

void write_symbol_id(Token *symb, char *symb_frame)
{
    append_string(CURRENT_BLOCK, symb_frame);
    append_string(CURRENT_BLOCK, "@");
    append_string(CURRENT_BLOCK, symb->str->str);
    append_string(CURRENT_BLOCK, " ");
}

void write_symbol_type(Token *symb)
{
    switch (symb->type)
    {
    case INT:
        append_string(CURRENT_BLOCK, "int");
        break;
    case DEC:
        append_string(CURRENT_BLOCK, "float");
        break;
    case STR:
        append_string(CURRENT_BLOCK, "string");
        break;
    default:
        break;
    }

    append_string(CURRENT_BLOCK, "@");
    append_string(CURRENT_BLOCK, symb->str->str);

    append_string(CURRENT_BLOCK, " ");
}

void write_symbol(Token *symb, char *symb_frame)
{
    if (symb->type == ID)
    {
        write_symbol_id(symb, symb_frame);
    }
    else if (symb->type >= DEC && symb->type <= STR)
    {
        write_symbol_type(symb);
    }
    else
    {
        global_error_code = INTERNAL_ERROR;
        print_internal_error(INTERNAL_ERROR, ERROR, "Typ tokenu mimo hranice enumu tokenu");
    }
}

void write_move(Token *op1, char *op1_frame, Token *res, char *res_frame)
{
    append_string(CURRENT_BLOCK, "MOVE ");

    //var
    write_symbol(res, res_frame);

    //symb
    write_symbol(op1, op1_frame);

    append_string(CURRENT_BLOCK, "\n");
}

void write_defvar(Token *res)
{
    insert_table_item(res->str->str, CURRENT_FRAME);

    append_string(CURRENT_BLOCK, "DEFVAR ");

    //var
    write_symbol(res, CURRENT_FRAME_STRING);

    append_string(CURRENT_BLOCK, "\n");
}

char *check_and_define(Token *token)
{
    bool token_exists_global = item_exists_table(token->str->str, GLOBAL_FRAME);

    char *token_frame_str;

    if (token_exists_global)
    {
        token_frame_str = get_frame_string(GLOBAL_FRAME);
    }
    else
    {
        if (CURRENT_FRAME == GLOBAL_FRAME)
        {
            write_defvar(token);
            token_frame_str = get_frame_string(GLOBAL_FRAME);
        }
        else
        {
            bool token_exists_current = item_exists_table(token->str->str, CURRENT_FRAME);

            if (!token_exists_current)
            {
                write_defvar(token);
            }
            token_frame_str = CURRENT_FRAME_STRING;
        }
    }

    return token_frame_str;
}

void write_assign(Token *op1, Token *res)
{
    char *op1_frame_str = check_and_define(op1);
    char *res_frame_str = check_and_define(res);

    write_move(op1, op1_frame_str, res, res_frame_str);
}

void write_arithmetic(ac_type type, Token *op1, Token *op2, Token *res)
{
    char *op1_frame_str = check_and_define(op1);
    char *op2_frame_str = check_and_define(op2);
    char *res_frame_str = check_and_define(res);

    switch (type)
    {
    case ADD:
        append_string(CURRENT_BLOCK, "ADD ");
        break;
    case SUB:
        append_string(CURRENT_BLOCK, "SUB ");
        break;
    case MUL:
        append_string(CURRENT_BLOCK, "MUL ");
        break;
    case DIV:
        append_string(CURRENT_BLOCK, "DIV ");
        break;
    case DIVINT:
        append_string(CURRENT_BLOCK, "IDIV ");
        break;
    default:
        break;
    }

    //res
    write_symbol(res, res_frame_str);

    //op1
    write_symbol(op1, op1_frame_str);

    //op2
    write_symbol(op2, op2_frame_str);
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
    res->type = ID;

    write_assign(op1, res);

    print_gen_all();

    free_gen();

    return 0;
}

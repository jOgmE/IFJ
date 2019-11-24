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

char *convert_int_to_string(int data)
{
    size_t length = snprintf(NULL, 0, "%d", data);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%d", data);
    return str;
}

char *convert_float_to_string(double data)
{
    size_t length = snprintf(NULL, 0, "%a", data);
    char *str = malloc(length + 1);
    snprintf(str, length + 1, "%a", data);
    return str;
}

char *convert_string(char *string)
{
    size_t len = strlen(string);

    cstring *result = init_cstring_size(1);

    for (size_t i = 0; i < len; i++)
    {
        if ((string[i] >= 0 && string[i] <= 32) || string[i] == 35 || string[i] == 92)
        {
            append_string(result, "\\");
            append_char(result, '0');
            append_string(result, convert_int_to_string(string[i]));
        }
        else
        {
            append_char(result, string[i]);
        }
    }

    return result->str;
}

char *get_table_key_from_token(Token *token)
{
    switch (token->type)
    {
    case STR:
    case ID:
        return token->str->str;
    case INT:
        return convert_int_to_string(token->i);
    case DEC:
        return convert_int_to_string(token->dec);
    default:
        global_error_code = INTERNAL_ERROR;
        print_internal_error(INTERNAL_ERROR, ERROR, "Typ tokenu mimo hranice moznych typu promennych a symbolu");
        return "";
    }
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
        append_string(CURRENT_BLOCK, "int@");
        append_string(CURRENT_BLOCK, convert_int_to_string(symb->i));
        break;
    case DEC:
        append_string(CURRENT_BLOCK, "float@");
        append_string(CURRENT_BLOCK, convert_float_to_string(symb->dec));
        break;
    case STR:
        append_string(CURRENT_BLOCK, "string@");
        append_string(CURRENT_BLOCK, convert_string(symb->str->str));
        break;
    default:
        break;
    }

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
    insert_table_item(get_table_key_from_token(res), NONE, CURRENT_FRAME);

    append_string(CURRENT_BLOCK, "DEFVAR ");

    //var
    write_symbol(res, CURRENT_FRAME_STRING);

    append_string(CURRENT_BLOCK, "\n");
}

Token *get_table_token(Token *token)
{
    char *token_key = token->str->str;

    if (item_exists_table(token_key, GLOBAL_FRAME))
    {
        return get_table_item_token(token_key, GLOBAL_FRAME);
    }
    else
    {
        return get_table_item_token(token_key, CURRENT_FRAME);
    }
}

e_type get_token_type(Token *token)
{
    char *token_key = token->str->str;

    if (item_exists_table(token_key, GLOBAL_FRAME))
    {
        return get_table_item_type(token_key, GLOBAL_FRAME);
    }
    else
    {
        return get_table_item_type(token_key, CURRENT_FRAME);
    }
}

void write_convert_type(Token *token, char *frame_str, e_type destType)
{
    if (token->type == destType)
    {
        return;
    }

    if (token->type != ID)
    {
        return;
    }

    Token *table_token = get_table_token(token);

    switch (table_token->type)
    {
    case INT:
        if (destType == DEC)
        {
            append_string(CURRENT_BLOCK, "INT2FLOAT ");
            write_symbol(token, frame_str);
            write_symbol(table_token, frame_str);
            // append_string(CURRENT_BLOCK, convert_int_to_string(table_token->i));
            append_string(CURRENT_BLOCK, "\n");
        }
        break;
    case DEC:
        if (destType == INT)
        {
            append_string(CURRENT_BLOCK, "FLOAT2INT ");
            write_symbol(token, frame_str);
            write_symbol(table_token, frame_str);
            // append_string(CURRENT_BLOCK, convert_float_to_string(table_token->dec));
            append_string(CURRENT_BLOCK, "\n");
        }
        break;
    default:
        break;
    }
}

char *write_check_and_define(Token *token)
{
    if (token->type != ID)
    {
        return "";
    }

    // char *token_key = get_table_key_from_token(token);
    char *token_key = token->str->str;

    bool token_exists_global = item_exists_table(token_key, GLOBAL_FRAME);

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
            bool token_exists_current = item_exists_table(token_key, CURRENT_FRAME);

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
    char *op1_frame_str = write_check_and_define(op1);
    char *res_frame_str = write_check_and_define(res);

    frame_t frame = item_exists_table(res->str->str, GLOBAL_FRAME) ? GLOBAL_FRAME : CURRENT_FRAME;

    update_table_item_token(res->str->str, op1, frame);

    write_move(op1, op1_frame_str, res, res_frame_str);
}

void write_arithmetic(ac_type type, Token *op1, Token *op2, Token *res)
{
    char *op1_frame_str = write_check_and_define(op1);
    char *op2_frame_str = write_check_and_define(op2);
    char *res_frame_str = write_check_and_define(res);

    e_type artihmetic_type = get_token_type(res);

    if (op1->type != op2->type)
    {
        // float type is lower in enum :(
        if (op1->type < artihmetic_type)
        {
            artihmetic_type = op1->type;
        }
        if (op2->type < artihmetic_type)
        {
            artihmetic_type = op2->type;
        }

        if (op1->type != ID && op1->type != artihmetic_type)
        {
            op1->type = artihmetic_type;

            if (artihmetic_type == DEC)
            {
                op1->dec = (double)op1->i;
            }
        }
        // if (op1->type != artihmetic_type && op1->type != ID)
        else if (op1->type != artihmetic_type)
        {
            write_convert_type(op1, op1_frame_str, artihmetic_type);
        }

        if (op2->type != ID && op2->type != artihmetic_type)
        {
            op2->type = artihmetic_type;

            if (artihmetic_type == DEC)
            {
                op2->dec = (double)op2->i;
            }
        }
        else if (op2->type != artihmetic_type)
        {
            write_convert_type(op2, op2_frame_str, artihmetic_type);
        }
    }

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
    Token *op2 = malloc(sizeof(Token));
    Token *res = malloc(sizeof(Token));

    cstring *op1_str = init_cstring("operatorOne");
    cstring *op2_str = init_cstring("operatorTwo");
    cstring *res_str = init_cstring("result");

    // op1->str = NULL;
    // op1->dec = 50.5050;
    // op1->type = DEC;

    op1->str = NULL;
    op1->i = 5;
    op1->type = INT;

    op2->str = op2_str;
    op2->type = ID;

    write_assign(op1, op2);

    op1->str = NULL;
    op1->dec = 5.6;
    op1->type = DEC;

    // op1->str = NULL;
    // op1->i = 1234;
    // op1->type = INT;

    // op2->str = NULL;
    // op2->i = 5678;
    // op2->type = INT;

    // op2->str = NULL;
    // op2->dec = 5.6;
    // op2->type = DEC;

    // res->str = res_str;
    // res->type = ID;

    res->str = res_str;
    res->i = 9;
    res->type = ID;

    write_arithmetic(ADD, op1, op2, op2);

    print_gen_all();

    free_gen();

    return 0;
}

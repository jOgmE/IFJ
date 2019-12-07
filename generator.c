/** @file generator.c
 *  Implementace generátoru výsledného kódu
 * 
 *  @author Jaroslav Hort (xhortj04)
 *  @date 2019
 */

#include "generator.h"

#define DEBUG

//******************************************************************************************//
//******************************************************************************************//
//********************                 Globalni promenne                      **************//
//******************************************************************************************//
//******************************************************************************************//

FILE *out_stream = NULL;
char *result_code_filename;

cstring *result_code = NULL;
cstring *result_main_function_code = NULL;
cstring *result_functions_code = NULL;

cstring *CURRENT_BLOCK;
frame_t PREVIOUS_FRAME;
frame_t CURRENT_FRAME = GLOBAL_FRAME;
char *CURRENT_FRAME_STRING;

bool function_call_assign = false;
bool function_definition = false;

bool inputi_appended = false;
bool inputf_appended = false;
bool inputs_appended = false;
bool len_appended = false;

size_t tmp_if_counter = 0;
size_t tmp_var_counter = 0;
size_t param_counter = 0;

#ifdef DEBUG

bool just_type = false;

#endif

//******************************************************************************************//
//******************************************************************************************//
//********************                   Inicializace                         **************//
//******************************************************************************************//
//******************************************************************************************//

#ifdef DEBUG

void init_gen_test(char *filename)
{
    cstring *file_name = init_cstring(filename);

    append_string(file_name, ".ic19");

    result_code_filename = file_name->str;

    out_stream = fopen(result_code_filename, "w");

    if (out_stream == NULL)
    {
        global_error_code = INTERNAL_ERROR;
        print_internal_error(INTERNAL_ERROR, ERROR, "Chyba vytvoreni vystupniho souboru");
        return;
    }

    init_gen();
}

#endif

void init_gen()
{
    result_code = init_cstring(".IFJcode19\nJUMP $$main\n\n");
    result_main_function_code = init_cstring("LABEL $$main\n");
    result_functions_code = init_cstring_size(1);

    CURRENT_FRAME = GLOBAL_FRAME;
    CURRENT_BLOCK = result_main_function_code;
    CURRENT_FRAME_STRING = get_frame_string(CURRENT_FRAME);

    init_table(32, GLOBAL_FRAME);
    init_table(16, LOCAL_FRAME);
    init_table(16, TEMP_FRAME);
}

void print_gen_all()
{
    if (global_error_code != SUCCESS)
    {
        return;
    }

    if (result_code != NULL)
    {
        fprintf(stdout, "%s", result_code->str);
    }

    if (result_functions_code != NULL)
    {
        fprintf(stdout, "%s", result_functions_code->str);
    }

    if (result_main_function_code != NULL)
    {
        fprintf(stdout, "%s", result_main_function_code->str);
    }
}

void free_gen()
{
    if (out_stream == NULL)
    {
        return;
    }

    clear_table(GLOBAL_FRAME);
    clear_table(LOCAL_FRAME);
    clear_table(TEMP_FRAME);

    free_cstring(result_code);
    free_cstring(result_functions_code);
    free_cstring(result_main_function_code);
}

void switch_frame(frame_t frame)
{
    CURRENT_FRAME = frame;
    CURRENT_FRAME_STRING = get_frame_string(frame);
}

//******************************************************************************************//
//******************************************************************************************//
//********************                 Prevody na string                      **************//
//******************************************************************************************//
//******************************************************************************************//

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

//******************************************************************************************//
//******************************************************************************************//
//********************         Pomocne operace (frame tabulky, tokeny)        **************//
//******************************************************************************************//
//******************************************************************************************//

char *get_table_key_from_token(Token *token)
{
    switch (token->type)
    {
    case STR:
    case ID:
    case TEMP_ID:
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

void change_token_types(Token *token, e_type arithmetic_type)
{
    if (token->type != arithmetic_type)
    {
        if (token->type == INT)
        {
            token->type = DEC;
            token->dec = (double)token->i;
        }
        else if (token->type == DEC)
        {
            token->type = INT;
            token->i = (int)token->dec;
        }
    }
}

e_type compare_symbol_types_and_convert(Token *op1, Token *op2)
{
    e_type arithmetic_type = INT;

    if (op1->type != ID)
    {
        arithmetic_type = op1->type;
    }

    if (op2->type != ID)
    {
        if (arithmetic_type > op2->type)
        {
            arithmetic_type = op2->type;

            if (op1->type != ID && arithmetic_type != op1->type)
            {
                if (arithmetic_type == INT)
                {
                    op1->type = INT;
                    op1->i = (int)op1->dec;
                }
                else if (arithmetic_type == DEC)
                {
                    op1->type = DEC;
                    op1->dec = (double)op1->i;
                }
            }
        }
        else if (arithmetic_type < op2->type)
        {
            op2->type = DEC;
            op2->dec = (double)op2->i;
        }
    }

    return arithmetic_type;
}

void definition_start()
{
    CURRENT_BLOCK = result_functions_code;
    PREVIOUS_FRAME = CURRENT_FRAME;
    switch_frame(LOCAL_FRAME);
    function_definition = true;
}

void definition_end()
{
    append_string(CURRENT_BLOCK, "POPFRAME\nRETURN\n");

    CURRENT_BLOCK = result_functions_code;
    frame_t temp_frame_var = CURRENT_FRAME;
    switch_frame(PREVIOUS_FRAME);
    PREVIOUS_FRAME = temp_frame_var;
    function_definition = false;
}

//******************************************************************************************//
//******************************************************************************************//
//********************                  Symboly                               **************//
//******************************************************************************************//
//******************************************************************************************//

void write_symbol_id(Token *symb, char *symb_frame, bool skip_space)
{
    append_string(CURRENT_BLOCK, symb_frame);
    append_string(CURRENT_BLOCK, "@");
    append_string(CURRENT_BLOCK, symb->str->str);

    if (!skip_space)
    {
        append_string(CURRENT_BLOCK, " ");
    }
}

void write_symbol_type(Token *symb, bool skip_space)
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

    if (!skip_space)
    {
        append_string(CURRENT_BLOCK, " ");
    }
}

void write_symbol(Token *symb, char *symb_frame, bool skip_space)
{
    if (symb->type == ID || symb->type == TEMP_ID)
    {
        write_symbol_id(symb, symb_frame, skip_space);
    }
    else if (symb->type >= DEC && symb->type <= STR)
    {
        write_symbol_type(symb, skip_space);
    }
    else
    {
        global_error_code = INTERNAL_ERROR;
        print_internal_error(INTERNAL_ERROR, ERROR, "Typ tokenu mimo hranice enumu tokenu");
    }
}

//******************************************************************************************//
//******************************************************************************************//
//********************                Zakladni operace                        **************//
//******************************************************************************************//
//******************************************************************************************//

void write_move(Token *op1, char *op1_frame, Token *res, char *res_frame)
{
    append_string(CURRENT_BLOCK, "MOVE ");

    //var
    write_symbol(res, res_frame, false);

    //symb
    write_symbol(op1, op1_frame, false);

    append_string(CURRENT_BLOCK, "\n");
}

void write_defvar(Token *res)
{
    insert_table_item(get_table_key_from_token(res), CURRENT_FRAME);

    append_string(CURRENT_BLOCK, "DEFVAR ");

    //var
    write_symbol(res, CURRENT_FRAME_STRING, false);

    append_string(CURRENT_BLOCK, "\n");
}

void write_label(char *label)
{
    bool label_exists = item_exists_table(label, GLOBAL_FRAME);

    if (!label_exists)
    {
        // TODO : token probably shouldn't be null
        insert_table_item(label, GLOBAL_FRAME);
    }
    else
    {
        append_string(CURRENT_BLOCK, "EXIT 3");
        return;
    }

    append_string(CURRENT_BLOCK, "LABEL ");
    append_string(CURRENT_BLOCK, label);
    append_string(CURRENT_BLOCK, "\n");

    if (function_definition)
    {
        append_string(CURRENT_BLOCK, "PUSHFRAME\n");
        append_string(CURRENT_BLOCK, "DEFVAR LF@%retval\n");
        append_string(CURRENT_BLOCK, "MOVE LF@%retval nil@nil\n");
    }
}

char *write_check_and_define(Token *token)
{
    if (token == NULL)
    {
        return "";
    }

    if (token->type != ID && token->type != TEMP_ID)
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

void write_create_frame()
{
    append_string(CURRENT_BLOCK, "CREATE FRAME\n");
    switch_frame(TEMP_FRAME);
}

void write_param(Token *res)
{
    if (param_counter == 0)
    {
        function_call_assign = true;
        write_create_frame();
    }

    ++param_counter;

    char *param_counter_str = convert_int_to_string((int)param_counter);

    char *res_frame_str = write_check_and_define(res);

    if (function_call_assign)
    {

        // if (CURRENT_FRAME != TEMP_FRAME)
        // {
        //     switch_frame(TEMP_FRAME);
        // }

        append_string(CURRENT_BLOCK, "DEFVAR TF@%");
        append_string(CURRENT_BLOCK, param_counter_str);
        append_string(CURRENT_BLOCK, "\n");

        append_string(CURRENT_BLOCK, "MOVE TF@%");
        append_string(CURRENT_BLOCK, param_counter_str);
        write_symbol(res, res_frame_str, true);
        append_string(CURRENT_BLOCK, "\n");
    }
    else
    {
        append_string(CURRENT_BLOCK, "DEFVAR ");
        append_string(CURRENT_BLOCK, res_frame_str);
        append_string(CURRENT_BLOCK, "\n");

        append_string(CURRENT_BLOCK, "MOVE ");
        write_symbol(res, res_frame_str, false);
        append_string(CURRENT_BLOCK, " LF@%\n");
        append_string(CURRENT_BLOCK, param_counter_str);
    }
}

void write_return(Token *res)
{
    append_string(CURRENT_BLOCK, "MOVE LF@%retval ");

    char *res_frame_str = write_check_and_define(res);

    write_symbol(res, res_frame_str, false);

    append_string(CURRENT_BLOCK, "\n");
}

void check_and_define_while()
{
    set_ac_breakpoint();

    ac_type type = readACtype();

    Token *op1 = NULL, *op2 = NULL, *res = NULL;

    append_string(CURRENT_BLOCK, "\n#while start\n");

    while (isACActive() && type != WHILE_END)
    {
        // if (type == WHILE_START)
        // {
        //     setACAct();
        //     check_and_define_while();
        // }

        op1 = readACop1();
        op2 = readACop2();
        res = readACres();

        write_check_and_define(op1);
        write_check_and_define(op2);
        write_check_and_define(res);

        type = readACtype();

        actAC();
    }

    goto_ac_breakpoint();
}

void write_call(char *label)
{
    bool label_exists = item_exists_table(label, GLOBAL_FRAME);

    if (label_exists)
    {
        append_string(CURRENT_BLOCK, "CALL ");
        append_string(CURRENT_BLOCK, label);

        switch_frame(LOCAL_FRAME);
        CURRENT_BLOCK = result_main_function_code;

        param_counter = 0;
    }
    else
    {
        if (strcmp(label, "inputi") == 0 && !inputi_appended)
        {
            append_string(result_main_function_code, INPUTI_FUNC);
            inputi_appended = true;
        }
        else if (strcmp(label, "inputs") == 0 && !inputs_appended)
        {
            append_string(result_main_function_code, INPUTS_FUNC);
            inputs_appended = true;
        }
        else if (strcmp(label, "inputf") == 0 && !inputf_appended)
        {
            append_string(result_main_function_code, INPUTF_FUNC);
            inputf_appended = true;
        }
        else if (strcmp(label, "len") == 0 && !len_appended)
        {
            append_string(result_main_function_code, LEN_FUNC);
            len_appended = true;
        }
        else
        {
            append_string(CURRENT_BLOCK, "EXIT 3");

            cstring *error_string = init_cstring("Funkce ");

            append_string(error_string, label);
            append_string(error_string, " nebyla definovana.");

            global_error_code = 4;
            print_compile_error(4, ERROR, 0, error_string->str);

            free_cstring(error_string);
        }
    }
}

void write_print(Token *res)
{
    char *res_frame_str = write_check_and_define(res);

    append_string(CURRENT_BLOCK, "WRITE ");
    write_symbol(res, res_frame_str, true);
    append_string(CURRENT_BLOCK, "\n");
}

//******************************************************************************************//
//******************************************************************************************//
//********************                   Aritmetika                           **************//
//******************************************************************************************//
//******************************************************************************************//

void write_convert_type(Token *token, char *frame_str, e_type destType)
{
    if (token->type == destType || token->type != ID)
    {
        return;
    }

    // frame_t prev_frame = CURRENT_FRAME;
    // switch_frame(LOCAL_FRAME);

    char *tmp_var_id = convert_int_to_string((int)tmp_var_counter);
    char *tmp_if_id = convert_int_to_string((int)tmp_if_counter);

    cstring *iflabel_float = init_cstring_size(1);

    append_string(iflabel_float, "$if$");
    append_string(iflabel_float, token->str->str);
    append_string(iflabel_float, "$true$float$");
    append_string(iflabel_float, tmp_if_id);
    append_string(iflabel_float, " ");

    cstring *iflabel_int = init_cstring_size(1);

    append_string(iflabel_int, "$if$");
    append_string(iflabel_int, token->str->str);
    append_string(iflabel_int, "$true$int$");
    append_string(iflabel_int, tmp_if_id);
    append_string(iflabel_int, " ");

    append_string(CURRENT_BLOCK, "DEFVAR ");
    write_symbol(token, frame_str, true);
    append_string(CURRENT_BLOCK, "$type$s");
    append_string(CURRENT_BLOCK, tmp_var_id);
    append_string(CURRENT_BLOCK, "\n");

    append_string(CURRENT_BLOCK, "DEFVAR ");
    write_symbol(token, frame_str, true);
    append_string(CURRENT_BLOCK, "$tmp$");
    append_string(CURRENT_BLOCK, tmp_var_id);
    append_string(CURRENT_BLOCK, "\n");

    append_string(CURRENT_BLOCK, "TYPE ");
    write_symbol(token, frame_str, true);
    append_string(CURRENT_BLOCK, "$type$");
    append_string(CURRENT_BLOCK, tmp_var_id);
    append_string(CURRENT_BLOCK, " ");
    write_symbol(token, frame_str, true);
    append_string(CURRENT_BLOCK, "\n");

    append_string(CURRENT_BLOCK, "JUMPIFEQ ");
    append_cstring(CURRENT_BLOCK, iflabel_int);
    write_symbol(token, frame_str, true);
    append_string(CURRENT_BLOCK, "$type$");
    append_string(CURRENT_BLOCK, tmp_var_id);
    append_string(CURRENT_BLOCK, " ");
    append_string(CURRENT_BLOCK, "string@int\n");

    append_string(CURRENT_BLOCK, "JUMPIFEQ ");
    append_cstring(CURRENT_BLOCK, iflabel_float);
    write_symbol(token, frame_str, true);
    append_string(CURRENT_BLOCK, "$type$");
    append_string(CURRENT_BLOCK, tmp_var_id);
    append_string(CURRENT_BLOCK, " ");
    append_string(CURRENT_BLOCK, "string@float\n");

    append_string(CURRENT_BLOCK, "EXIT int@4\n");

    if (destType == DEC)
    {
        write_label(iflabel_int->str);
        append_string(CURRENT_BLOCK, "INT2FLOAT ");
        write_symbol(token, frame_str, true);
        append_string(CURRENT_BLOCK, "$tmp$");
        append_string(CURRENT_BLOCK, tmp_var_id);
        append_string(CURRENT_BLOCK, " ");
        write_symbol(token, frame_str, true);
        append_string(CURRENT_BLOCK, "\n");
        write_label(iflabel_float->str);
    }
    else if (destType == INT)
    {
        write_label(iflabel_float->str);
        append_string(CURRENT_BLOCK, "FLOAT2INT ");
        write_symbol(token, frame_str, true);
        append_string(CURRENT_BLOCK, "$tmp$");
        append_string(CURRENT_BLOCK, tmp_var_id);
        append_string(CURRENT_BLOCK, " ");
        write_symbol(token, frame_str, true);
        append_string(CURRENT_BLOCK, "\n");
        write_label(iflabel_int->str);
    }

    free_cstring(iflabel_float);
    free_cstring(iflabel_int);

    ++tmp_if_counter;
    ++tmp_var_counter;

    // switch_frame(prev_frame);
}

void write_assign(Token *op1, Token *res)
{
    char *res_frame_str = write_check_and_define(res);

    char *op1_frame_str = write_check_and_define(op1);

    write_move(op1, op1_frame_str, res, res_frame_str);
}

void write_jump(Token *res)
{
    append_string(CURRENT_BLOCK, "JUMP ");
    append_string(CURRENT_BLOCK, res->str->str);
    append_string(CURRENT_BLOCK, "\n");
}

void write_cond_jump(Token *op1, Token *res)
{
    char *op1_frame_str = write_check_and_define(op1);

    append_string(CURRENT_BLOCK, "JUMPIFEQ ");
    append_string(CURRENT_BLOCK, res->str->str);
    append_string(CURRENT_BLOCK, " ");
    write_symbol(op1, op1_frame_str, false);
    append_string(CURRENT_BLOCK, "bool@true\n");
}

void write_comparison(ac_type type, Token *op1, Token *op2, Token *res)
{
    char *op1_frame_str = write_check_and_define(op1);
    char *op2_frame_str = write_check_and_define(op2);
    char *res_frame_str = write_check_and_define(res);

    e_type arithmetic_type = compare_symbol_types_and_convert(op1, op2);

    bool op1_converted = false;
    bool op2_converted = false;

    char *tmp_var_id = convert_int_to_string((int)tmp_var_counter);

    if (op1->type == ID)
    {
        write_convert_type(op1, op1_frame_str, arithmetic_type);
        op1_converted = true;
    }

    if (op2->type == ID)
    {
        write_convert_type(op2, op2_frame_str, arithmetic_type);
        op2_converted = true;
    }

    if (type == LE || type == GE)
    {
        append_string(CURRENT_BLOCK, "DEFVAR ");
        write_symbol(res, res_frame_str, true);
        append_string(CURRENT_BLOCK, "$tmpeq$");
        append_string(CURRENT_BLOCK, tmp_var_id);
        append_string(CURRENT_BLOCK, "\n");

        append_string(CURRENT_BLOCK, "EQ ");
        write_symbol(res, res_frame_str, true);
        append_string(CURRENT_BLOCK, "$tmpeq$");
        append_string(CURRENT_BLOCK, tmp_var_id);
        append_string(CURRENT_BLOCK, "\n");

        write_symbol(op1, op1_frame_str, true);
        if (op1_converted)
        {
            append_string(CURRENT_BLOCK, "$tmp$");
            append_string(CURRENT_BLOCK, tmp_var_id);
            append_string(CURRENT_BLOCK, " ");
        }
        else
        {
            append_string(CURRENT_BLOCK, " ");
        }

        write_symbol(op2, op2_frame_str, true);
        if (op2_converted)
        {
            append_string(CURRENT_BLOCK, "$tmp$");
            append_string(CURRENT_BLOCK, tmp_var_id);
            append_string(CURRENT_BLOCK, "\n");
        }
        else
        {
            append_string(CURRENT_BLOCK, "\n");
        }

        append_string(CURRENT_BLOCK, "DEFVAR ");
        write_symbol(res, res_frame_str, true);

        if (type == LE)
        {
            append_string(CURRENT_BLOCK, "$tmplt$");
            append_string(CURRENT_BLOCK, tmp_var_id);
            append_string(CURRENT_BLOCK, " ");
        }
        else
        {
            append_string(CURRENT_BLOCK, "$tmpgt$");
            append_string(CURRENT_BLOCK, tmp_var_id);
            append_string(CURRENT_BLOCK, " ");
        }

        append_string(CURRENT_BLOCK, convert_int_to_string((int)tmp_var_counter));

        ++tmp_var_counter;

        append_string(CURRENT_BLOCK, "\n");
    }

    //res
    switch (type)
    {
    case EQUAL:
    case NOT_EQUAL:
        append_string(CURRENT_BLOCK, "EQ ");
        break;
    case LESS:
    case LE:
        append_string(CURRENT_BLOCK, "LT ");
        break;
    case GREATER:
    case GE:
        append_string(CURRENT_BLOCK, "GT ");
        break;
    default:
        break;
    }

    write_symbol(res, res_frame_str, true);
    if (type == LE)
    {
        append_string(CURRENT_BLOCK, "$tmplt$");
        append_string(CURRENT_BLOCK, tmp_var_id);
        append_string(CURRENT_BLOCK, " ");
    }
    else if (type == GE)
    {
        append_string(CURRENT_BLOCK, "$tmpgt$");
        append_string(CURRENT_BLOCK, tmp_var_id);
        append_string(CURRENT_BLOCK, " ");
    }
    else
    {
        append_string(CURRENT_BLOCK, " ");
    }

    //symb1
    write_symbol(op1, op1_frame_str, true);
    if (op1_converted)
    {
        append_string(CURRENT_BLOCK, "$tmp$");
        append_string(CURRENT_BLOCK, tmp_var_id);
        append_string(CURRENT_BLOCK, " ");
    }
    else
    {
        append_string(CURRENT_BLOCK, " ");
    }

    //symb2
    write_symbol(op2, op2_frame_str, true);
    if (op2_converted)
    {
        append_string(CURRENT_BLOCK, "$tmp$");
        append_string(CURRENT_BLOCK, tmp_var_id);
        append_string(CURRENT_BLOCK, " ");
    }
    else
    {
        append_string(CURRENT_BLOCK, " ");
    }

    append_string(CURRENT_BLOCK, "\n");

    if (type == NOT_EQUAL)
    {
        append_string(CURRENT_BLOCK, "NOT ");
        write_symbol(res, res_frame_str, false);
        write_symbol(res, res_frame_str, true);
        append_string(CURRENT_BLOCK, "\n");
    }
    else if (type == LE || type == GE)
    {
        append_string(CURRENT_BLOCK, "OR ");
        write_symbol(res, res_frame_str, true);
        append_string(CURRENT_BLOCK, "$tmpeq$");
        append_string(CURRENT_BLOCK, tmp_var_id);
        append_string(CURRENT_BLOCK, " ");
        write_symbol(res, res_frame_str, true);

        if (type == LE)
        {
            append_string(CURRENT_BLOCK, "$tmplt$");
            append_string(CURRENT_BLOCK, tmp_var_id);
            append_string(CURRENT_BLOCK, "\n");
        }
        else
        {
            append_string(CURRENT_BLOCK, "$tmpgt$");
            append_string(CURRENT_BLOCK, tmp_var_id);
            append_string(CURRENT_BLOCK, "\n");
        }
    }

    ++tmp_var_counter;
}

void write_arithmetic(ac_type type, Token *op1, Token *op2, Token *res)
{
    char *op1_frame_str = write_check_and_define(op1);
    char *op2_frame_str = write_check_and_define(op2);
    char *res_frame_str = write_check_and_define(res);

    e_type arithmetic_type = INT;

    char *tmp_var_id = convert_int_to_string((int)tmp_var_counter);

    bool op1_converted = false;
    bool op2_converted = false;

    if (type != DIV && type != DIVINT)
    {
        arithmetic_type = compare_symbol_types_and_convert(op1, op2);
    }
    else
    {
        if (type == DIV)
        {
            arithmetic_type = DEC;
        }
        else if (type == DIVINT)
        {
            arithmetic_type = INT;
        }
        change_token_types(op1, arithmetic_type);
        change_token_types(op2, arithmetic_type);
    }

    if (op1->type == ID)
    {
        write_convert_type(op1, op1_frame_str, arithmetic_type);
        op1_converted = true;
    }

    if (op2->type == ID)
    {
        write_convert_type(op2, op2_frame_str, arithmetic_type);
        op2_converted = true;
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
    write_symbol(res, res_frame_str, false);

    //op1
    write_symbol(op1, op1_frame_str, true);
    if (op1_converted)
    {
        append_string(CURRENT_BLOCK, "$tmp$");
        append_string(CURRENT_BLOCK, tmp_var_id);
        append_string(CURRENT_BLOCK, " ");
    }
    else
    {
        append_string(CURRENT_BLOCK, " ");
    }

    //op2
    write_symbol(op2, op2_frame_str, true);
    if (op2_converted)
    {
        append_string(CURRENT_BLOCK, "$tmp$");
        append_string(CURRENT_BLOCK, tmp_var_id);
        append_string(CURRENT_BLOCK, " ");
    }
    else
    {
        append_string(CURRENT_BLOCK, " ");
    }

    ++tmp_var_counter;
}

//******************************************************************************************//
//******************************************************************************************//
//********************                  Hlavni funkce                         **************//
//******************************************************************************************//
//******************************************************************************************//

#ifdef DEBUG

void print_type(ac_type type)
{
    switch (type)
    {
    case LABEL:
        printf("LABEL");
        break;
    case RET:
        printf("RET");
        break;
    case CALL:
        printf("CALL");
        break;
    case JUMP:
        printf("JUMP");
        break;
    case COND_JUMP:
        printf("COND_JUMP");
        break;
    case ADD:
        printf("ADD");
        break;
    case SUB:
        printf("SUB");
        break;
    case MUL:
        printf("MUL");
        break;
    case DIV:
        printf("DIV");
        break;
    case DIVINT:
        printf("DIVINT");
        break;
    case ASSIGN:
        printf("ASSIGN");
        break;
    case EQUAL:
        printf("EQUAL");
        break;
    case NOT_EQUAL:
        printf("NOT_EQUAL");
        break;
    case GREATER:
        printf("GREATER");
        break;
    case GE:
        printf("GE");
        break;
    case LESS:
        printf("LESS");
        break;
    case LE:
        printf("LE");
        break;
    case DEF_START:
        printf("DEF_START");
        break;
    case DEF_END:
        printf("DEF_END");
        break;
    case PARAM:
        printf("PARAM");
        break;
    case WHILE_START:
        printf("WHILE_START");
        break;
    case WHILE_END:
        printf("WHILE_END");
        break;
    case UNDEFINED:
        printf("UNDEFINED");
        break;
    }
}

void print_ac(tAC *ac)
{
    if (just_type)
    {
        printf("AC: ");
        print_type(ac->type);
        printf("\n");
        if (ac->op1 != NULL)
            printf("  1 %s\n", get_cstring_string(ac->op1->str));
        if (ac->op2 != NULL)
            printf("  2 %s\n", get_cstring_string(ac->op2->str));
        if (ac->res != NULL)
            printf("  r %s\n", get_cstring_string(ac->res->str));
        return;
    }
    printf("Adress code struct\n    type - %d\n", ac->type);
    printf("    Token op1 - %p\n", (void *)ac->op1);
    if (ac->op1 != NULL)
    {
        printf("          dbl - %f\n          int - %d\n          typ - %d\n", ac->op1->dec, ac->op1->i, ac->op1->type);
        if (ac->op1->str != NULL)
        {
            const char *str = get_cstring_string(ac->op1->str);
            printf("          str - %s\n", str);
        }
        else
            printf("          str - not\n");
    }
    printf("    Token op2 - %p\n", (void *)ac->op2);
    if (ac->op2 != NULL)
    {
        printf("          dbl - %f\n          int - %d\n          typ - %d\n", ac->op2->dec, ac->op2->i, ac->op2->type);
        if (ac->op2->str != NULL)
        {
            const char *str = get_cstring_string(ac->op2->str);
            printf("          str - %s\n", str);
        }
        else
            printf("          str - not\n");
    }
    printf("    Token op1 - %p\n", (void *)ac->res);
    if (ac->res != NULL)
    {
        printf("          dbl - %f\n          int - %d\n          typ - %d\n", ac->res->dec, ac->res->i, ac->res->type);
        if (ac->res->str != NULL)
        {
            const char *str = get_cstring_string(ac->res->str);
            printf("          str - %s\n", str);
        }
        else
            printf("          str - not\n");
    }
}

#endif

void generate_code()
{

#ifdef DEBUG

    just_type = true;

    setACAct();
    while (isACActive())
    {
        print_ac(readAC());
        actAC();
    }

    printf("\n\n");

#endif

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
        case ADD:
        case SUB:
        case MUL:
        case DIVINT:
            write_arithmetic(type, op1, op2, res);
            break;
        case EQUAL:
        case NOT_EQUAL:
        case GREATER:
        case GE:
        case LESS:
        case LE:
            write_comparison(type, op1, op2, res);
            break;
        case LABEL:
            write_label(res->str->str);
            break;
        case JUMP:
            write_jump(res);
            break;
        case COND_JUMP:
            write_cond_jump(op1, res);
            break;
        case DEF_START:
            definition_start();
            break;
        case DEF_END:
            definition_end();
            break;
        case CALL:
            function_call_assign = false;
            write_call(res->str->str);
            break;
        case WHILE_START:
            check_and_define_while();
            break;
        case WHILE_END:
            break;
        case PARAM:
            write_param(res);
            break;
        default:
            break;
        }

        actAC();
    }

    print_gen_all();
}

#ifdef DEBUG

//******************************************************************************************//
//******************************************************************************************//
//********************              !!!!TESTOVANI, SMAZAT!!!                  **************//
//******************************************************************************************//
//******************************************************************************************//

/*
int main(int argc, char const *argv[])
{
    init_gen_test("testout");

    Token *op1 = malloc(sizeof(Token));
    Token *op2 = malloc(sizeof(Token));
    Token *res = malloc(sizeof(Token));

    //cstring *op1_str = init_cstring("operatorOne");
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

    fclose(out_stream);

    return 0;
}
*/

#endif
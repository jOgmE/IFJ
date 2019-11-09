/** @file errors.h
 *  Implementation of library for error handling
 *  
 *  Created as a part of IFJcode19 interpreter for IFJ course at BUT FIT
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */

#include "errors.h"

// Extern definitions
error_type global_error_code = SUCCESS;

const char *error_type_names[] = {"SUCCESS", "NULL_ERROR", "MEMORY_ERROR", "INVALID_POINTER", "TOKEN_ERROR"};

const char *error_sev_names[] = {"INFO", "WARNING", "ERROR"};

error_type init_error_list(error_list *err_list)
{
    if (err_list == NULL)
    {
        return NULL_ERROR;
    }

    err_list->length = 0;
    err_list->first_error = NULL;

    return SUCCESS;
}

error_type init_error(error *err, error_type type, error_sev severity, char *message)
{
    if (err == NULL)
    {
        return NULL_ERROR;
    }

    err->next_err = err->prev_err = NULL;

    err->message = message;
    err->severity = severity;
    err->type = type;

    return SUCCESS;
}

error_type add_error(error_list *err_list, error *err)
{
    if (err_list == NULL || err == NULL)
    {
        return NULL_ERROR;
    }

    error *current_err = err_list->first_error;

    if (current_err == NULL)
    {
        err_list->first_error = err;
        err_list->length = 1;
        return SUCCESS;
    }

    while (current_err->next_err != NULL)
    {
        current_err = current_err->next_err;
    }

    current_err->next_err = err;
    ++(err_list->length);

    return SUCCESS;
}

error_type init_and_add_error(error_list *err_list, error_type type, error_sev severity, char *message)
{
    if (err_list == NULL)
    {
        return NULL_ERROR;
    }

    error *err = (error *)malloc(sizeof(error));

    if (err == NULL)
    {
        return MEMORY_ERROR;
    }

    error_type init_result = init_error(err, type, severity, message);

    if (init_result != SUCCESS)
    {
        return init_result;
    }

    error_type add_result = add_error(err_list, err);

    if (add_result != SUCCESS)
    {
        return add_result;
    }

    return SUCCESS;
}

error_type print_errors(error_list *err_list)
{
    if (err_list == NULL)
    {
        return NULL_ERROR;
    }

    error *current_err = err_list->first_error;

    size_t printIndex = 1;
    while (current_err != NULL)
    {
        // TODO Format message
        printf("[%zu] [%s] %s\n", printIndex, error_type_names[current_err->type], current_err->message);
        ++printIndex;
        current_err = current_err->next_err;
    }

    return SUCCESS;
}
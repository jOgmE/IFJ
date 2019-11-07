/** @file dynamic_string.c
 *  Implementation of library for dynamically allocated string
 *  
 *  Created as a part of IFJcode19 interpreter for IFJ course at BUT FIT
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */

#include "dynamic_string.h"

cstring *init_cstring_size(size_t size)
{

    cstring *cstr = (cstring *)malloc(sizeof(cstring));

    // Check if cstring struct was allocated succesfully
    if (cstr == NULL)
    {
        //TODO: Handle alloc errors
        return NULL;
    }

    cstr->str = (char *)malloc(size);

    // Check if string was allocated succesfully
    if (cstr->str == NULL)
    {
        //TODO: Handle alloc errors
        return NULL;
    }

    cstr->length = 0;
    cstr->allocated_size = size;

    return cstr;
}

cstring *init_cstring(char *str)
{
    size_t size = sizeof(str);

    cstring *cstr = init_cstring_size(size);

    memcpy(cstr->str, str, size);

    cstr->length = strlen(str);

    return cstr;
}

error_type resize_cstring(cstring *cstr, size_t new_size)
{
    if (cstr->allocated_size > new_size)
    {
        cstr->str[new_size] = '\0';
    }
    else if (cstr->allocated_size == new_size) // No need to resize
    {
        return SUCCESS;
    }

    cstr->str = (char *)realloc(cstr->str, new_size);

    // Check if string was reallocated succesfully
    if (cstr->str == NULL)
    {
        return MEMORY_ERROR;
    }

    cstr->allocated_size = new_size;
    cstr->length = strlen(cstr->str);

    return SUCCESS;
}

void append_char(cstring *cstr, char c)
{
    // Resize cstr->str string if char c cannot fit
    if (cstr->allocated_size < cstr->length + 1)
    {
        resize_cstring(cstr, cstr->allocated_size + 1);
    }

    // Concatenate string and character
    cstr->str[cstr->length] = c;
    ++cstr->length;
    cstr->str[cstr->length] = '\0';
}

void append_string(cstring *cstr, char *str)
{
    size_t new_size = cstr->length + strlen(str);

    if (cstr->allocated_size < new_size)
    {
        resize_cstring(cstr, new_size);
        cstr->allocated_size = new_size;
    }

    strcat(cstr->str, str);
    cstr->length = new_size;
    cstr->str[cstr->length] = '\0';
}

void append_cstring(cstring *dest, cstring *src)
{
    size_t new_size = dest->length + src->length;

    // Resize if string src can't fit in dest
    if (dest->allocated_size < new_size)
    {
        resize_cstring(dest, new_size);
        dest->allocated_size = new_size;
    }

    strcat(dest->str, src->str);

    dest->length = new_size;
}

bool compare_string(cstring *cstr, char *str)
{
    return strcmp(cstr->str, str) == 0;
}

bool compare_cstring(cstring *cstr1, cstring *cstr2)
{
    //Check if strings in cstrings are equal
    bool str_comparison = compare_string(cstr1, cstr2->str);

    bool allocated_size_comparison = cstr1->allocated_size == cstr2->allocated_size;

    bool length_comparison = cstr1->length == cstr2->length;

    bool equal = allocated_size_comparison && length_comparison && str_comparison;

    return equal;
}

void free_cstring(cstring *cstr)
{
    //Free string in cstring first
    free(cstr->str);

    free(cstr);
}
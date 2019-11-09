/** @file dynamic_string.h
 *  Library for dynamically allocated string
 *  
 *  Created as a part of IFJcode19 interpreter for IFJ course at BUT FIT
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <stdlib.h>  //malloc, realloc, free
#include <string.h>  //strlen, strcmp. memcpy
#include <stdbool.h> //bool type

#include "errors.h" //errors

/**
 * cstring structure
 */
typedef struct cstring_struct
{
    char *str;             // Pointer to allocated string of cstring struct
    size_t length;         // Length of string of cstring struct
    size_t allocated_size; // Allocated size of string of cstring struct
} cstring;

/**
 * Initializes new instance of cstring
 * 
 * @param cstr Pointer to allocated cstring
 * @param str Pointer, or string to data to initialize cstring to
 * @returns SUCCESS if initialized successfully or error_type if initialization failed
 */
error_type init_cstring(cstring *cstr, char *str);

/**
 * Initializes new instance of cstring  with string of specified size
 * 
 * @param cstr Pointer to allocated cstring
 * @param size Size of string in cstring
 * @returns SUCCESS if initialized successfully or error_type if initialization failed
 */
error_type init_cstring_size(cstring *cstr, size_t size);

/**
 * Resizes string to new size
 * 
 * @param cstr Pointer to initialized cstring
 * @param str Pointer to previously allocated cstring
 * @returns SUCCESS if resized successfully or error_type if resize failed
 */
error_type resize_cstring(cstring *cstr, size_t new_size);

/**
 * Appends new character to the end of cstring
 * 
 * @param cstr Pointer to initialized cstring
 * @param c Character to append
 * @returns SUCCESS if appended successfully or error_type if append failed
 */
error_type append_char(cstring *cstr, char c);

/**
 * Appends new character to the end of cstring
 * 
 * @param cstr Pointer to previously allocated cstring
 * @param str Pointer to previously allocated string to be appended
 * @returns SUCCESS if appended successfully or error_type if append failed
 */
error_type append_string(cstring *cstr, char *str);

/**
 * Appends cstring to the end of first cstring
 * 
 * @param dest Pointer to previously allocated cstring to append to
 * @param src Pointer to previously allocated cstring to be append
 * @returns SUCCESS if appended successfully or error_type if append failed
 */
error_type append_cstring(cstring *dest, cstring *src);

/**
 * Compares string of cstring with specified string
 * 
 * @param cstr Pointer to previouslly allocated cstring to compare
 * @param str Pointer to previouslly allocated string to compare
 * @returns True if cstring string is equal to str
 */
bool compare_string(cstring *cstr, char *str);

/**
 * Compares two instances of cstring
 * 
 * @param cstr1 Pointer to first previouslly allocated cstring to compare
 * @param cstr2 Pointer to second previouslly allocated cstring to compare
 * @returns True if instances of cstring are equal
 */
bool compare_cstring(cstring *cstr1, cstring *cstr2);

/**
 * Frees previously allocated cstring
 * 
 * @param str Pointer to previously allocated cstring
 * @returns SUCCESS if free'd successfully or error_type if free'd failed
 */
error_type free_cstring(cstring *cstr);

#endif // DYNAMIC_STRING_H
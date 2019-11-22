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
#include "errors.h"

//size of the allocation chunk
#define DYN_CHUNK_SIZE 256

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
 * @param str Pointer, or string to data to initialize cstring to
 * @returns Pointer to allocated cstring
 */
cstring *init_cstring(const char *str);

/**
 * Initializes new instance of cstring  with string of specified size
 * The size is without '\0' at the end.
 * 
 * @param size Size of string in cstring
 * @returns Pointer to allocated cstring
 */
cstring *init_cstring_size(size_t size);

/**
 * Resizes string to new size
 * 
 * @param str Pointer to previously allocated cstring
 */
void resize_cstring(cstring *cstr, size_t new_size);

/**
 * Appends new character to the end of cstring
 * 
 * @param cstr Pointer to previously allocated cstring
 * @param c Character to append
 */
void append_char(cstring *cstr, char c);

/**
 * Appends new character to the end of cstring
 * 
 * @param cstr Pointer to previously allocated cstring
 * @param str Pointer to previously allocated string to be appended
 */
void append_string(cstring *cstr, char *str);

/**
 * Appends cstring to the end of first cstring
 * 
 * @param str Pointer to previously allocated cstring to append to
 * @param str Pointer to previously allocated cstring to be append
 */
void append_cstring(cstring *dest, cstring *src);

/**
 * Compares string of cstring with specified string
 * 
 * @param cstr Pointer to previouslly allocated cstring to compare
 * @param str Pointer to previouslly allocated string to compare
 * @returns True if cstring string is equal to str
 */
bool compare_string(cstring *cstr, const char *str);

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
 */
void free_cstring(cstring *cstr);

#endif // DYNAMIC_STRING_H

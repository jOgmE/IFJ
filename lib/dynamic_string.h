/** @file dynamic_string.h
 *  Library for dynamically allocated string
 *  
 *  Created as a part of IFJcode19 interpreter for IFJ course at BUT FIT
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */

#include <stdlib.h>  //malloc, realloc, free
#include <string.h>  //strlen, strcmp
#include <stdbool.h> //bool type

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
cstring *init_cstring(char *str);

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
 * Appends cstring to the end of first cstring
 * 
 * @param str Pointer to previously allocated cstring
 * @param c Character to append
 */
void append_cstring(cstring *dest, cstring *src);

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
 */
void free_cstring(cstring *cstr);
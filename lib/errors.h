/** @file errors.h
 *  Library for error handling
 *  
 *  Created as a part of IFJcode19 interpreter for IFJ course at BUT FIT
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */
#ifndef ERRORS_H

/**
 * LIB_ERROR enum
 */
typedef enum Errors
{
    SUCCESS,         // No error
    NULL_ERROR,      // Expression result is NULL
    MEMORY_ERROR,    // Memory allocation failed
    INVALID_POINTER, // Pointer to invalid memory
    TOKEN_ERROR      // Invalid token
} error_t;

/**
 * Check whether pointer points to valid memory 
 * 
 * @param ptr Pointer to memory to vech
 * @returns SUCCESS if valid, INVALID_POINTER if invalid
 */
error_t check_pointer(void *ptr);

#endif // ERRORS_H
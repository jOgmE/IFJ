/** @file errors.h
 *  Library for error handling
 *  
 *  Created as a part of IFJcode19 interpreter for IFJ course at BUT FIT
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */
#ifndef ERRORS_H
#define ERRORS_H

/**
 * error_type, general erros that can occur, functions should return following errors
 */
typedef enum ErrorTypes
{
    SUCCESS,         // No error
    NULL_ERROR,      // Expression result is NULL
    MEMORY_ERROR,    // Memory allocation failed
    INVALID_POINTER, // Pointer to invalid memory
    TOKEN_ERROR      // Invalid token
} error_type;

/**
 * error_severity
 */
typedef enum ErrorSeverity
{
    INFO,    // Information
    WARNING, // Warning, can compile
    ERROR    // Warning, cannot compile
} error_sev;

/**
 * Error type to be used in error list for compile error
 */
typedef struct Error
{
    error_type type;    // Error type
    error_sev severity; // Error severity
    char *message;      // Error Message
} error;

/**
 * Double linked list of errors
 */
typedef struct ErrorList
{
    error *next_err_ptr; // Pointer to next error
    error *prev_err_ptr; // Pointer to previous error
    int length;          // Lenght of list
} error_list;

/**
 * Initializes new list of errors
 * 
 * @param err_list Pointer to allocated memory for list
 * @returns SUCCESS if initialized successfully or error_type if initialization failed
 */
error_type init_error_list(error_list *err_list);

/**
 * Initializes new error struct
 * 
 * @param err Pointer to allocated memory for error
 * @param type Type of error
 * @param severity Severity of error
 * @param message Message to print (error description)
 * @returns SUCCESS if initialized successfully or error_type if initialization failed
 */
error_type init_error(error *err, error_type type, error_sev severity, char *message);

/**
 * Adds error err to a list of errors err_list
 * 
 * @param err_list Pointer to initialized list of errors
 * @param err Error to add to list
 * @returns SUCCESS if added successfully or error_type if add failed
 */
error_type add_error(error_list *err_list, error *err);

/**
 * Initializes new error and adds it to a list of errors
 * 
 * @param err_list List of errors to add new error to
 * @param err Pointer to allocated memory for error
 * @param type Type of error
 * @param severity Severity of error
 * @param message Message to print (error description)
 * @returns SUCCESS if initialized and added successfully or error_type if initialization and add failed
 */
error_type init_and_add_error(error_list *err_list, error_type type, error_sev severity, char *message);

/**
 * Prints all error in error list
 * 
 * @param err_list List of errors to be printed
 * @returns SUCCESS if printed succesfully or error_type if print failed
 */
error_type print_errors(error_list *err_list);

#endif // ERRORS_H
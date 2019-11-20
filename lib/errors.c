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

bool kill_after_analysis = false;

const char *error_type_names[] = {"SUCCESS", "NULL_ERROR", "MEMORY_ERROR", "INVALID_POINTER", "TOKEN_ERROR"};

const char *error_sev_names[] = {"INFO", "WARNING", "ERROR"};
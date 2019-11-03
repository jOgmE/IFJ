/** @file errors.h
 *  Implementation of library for error handling
 *  
 *  Created as a part of IFJcode19 interpreter for IFJ course at BUT FIT
 * 
 *  @author Jaroslav Hort
 *  @date 2019
 */

#include "errors.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

error_t check_pointer(void *ptr)
{
    int ptr_open = open(ptr, 0, 0);

    // Invalid memory
    if (ptr_open == -1 && errno == EFAULT)
    {
        return INVALID_POINTER;
    }

    close(ptr_open);

    return SUCCESS;
}

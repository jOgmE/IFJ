/** @parser.h
 * Syntakticka analyza rekurzivnim sestupem vseho krom vyrazu
 *
 * @author xstrna14
 * @version 1.0
 * @date 27.11.2019
 */

 #ifndef _PARSER_H_
 #define _PARSER_H_

#include "scanner.h"
#include "token.h"
#include "symtable.h"
#include "dynamic_string.h"
#include "errors.h"
#include "adress_code.h"
#include <stdbool.h>
//#include "" flippz something

extern bool kill_after_analysis;
extern error_type global_error_code;
extern size_t line_count;

void parser_do_magic();

 #endif //_PARSER_H_

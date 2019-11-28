/** @parser.h
 * Syntakticka analyza rekurzivnim sestupem vseho krom vyrazu
 *
 * @author xstrna14
 * @version 1.0
 * @date 27.11.2019
 */

 #ifndef _PARSER_H_
 #define _PARSER_H_

#include "../scanner/scanner.h"
#include "../scanner/token.h"
#include "symtable.h"
#include "dynamic_string.h"
#include "errors.h"
#include "adress_code.h"
//#include "" flippz something

//extern bool kill_after_analysis;
//extern bool global_error_code;

void parser_do_magic();

 #endif //_PARSER_H_

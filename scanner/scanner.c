/* @file scanner.c
 *
 * The core file of the scanner. It's implementing the FSM.
 * Characters readed in are converted into tokens.
 *
 * @autor xpocsn00
 * @date 2019-11-92
 * @version 0.1
 */

#include "scanner.h"

size_t line_count = 1;
static bool scanner_first_token = true; //first token being readed
static unsigned ws = 0; //ws counter
static tIndentStack *stack;

int getToken(FILE *f, Token *token){
    //checking static variable
    if(stack == NULL){
        stack = indentStackInit();
    }

    //declaring variables
    int present_state = S; //the actual state
    char resolved = -1; //ends the FSM
    int c = 0; //character to be readed
    bool dedent = false; //true when the last token was dedent
    //initialize it where needed
    cstring *our_string;
    //our_string is initialized in the (first from S) states:
    //F4
    //F5
    //Q2
    //F22
    //F11
    //F13
    //F15
    //Q9

    if(ws){ //ws (!= 0) was not cleared => dedent was the last token
        present_state = F1;
        dedent = true;
    }

    //ends while when resolved == 0
    while(resolved){
        if((c = fgetc(f)) == EOF){
            add_simple_data(token, EOFILE);
            --line_count; //is this okay?
            indentStackDestroy(stack);
            stack = NULL;
            return 0;
        }
        
        //FSM
        switch(present_state){
            //-------------------------STARTING STATE-------------------------------
            case S:
                if(c == 13) c = fgetc(f); //fcking windows with \r\n
                if(c == 10){ present_state = scanner_first_token ? S : F2; break;} //\n
                if(isSpace(c)){
                    present_state = scanner_first_token ? F1 : S;
                    if(scanner_first_token) ws++;
                    break;
                } //incrementing ws
                if(isIdNameStart(c)){
                    if((our_string = init_cstring_size(256)) == NULL){
                        global_error_code = INTERNAL_ERROR;
                        return INTERNAL_ERROR; //wihtout \0
                    }
                    append_char(our_string, c);
                    present_state = F4;
                    scanner_first_token = false;
                    break;
                }
                if(c == 48){
                    present_state = F8;
                    scanner_first_token = false;
                    break;
                } // 0
                if(is19num(c)){
                    if((our_string = init_cstring_size(32)) == NULL){
                        global_error_code = INTERNAL_ERROR;
                        return INTERNAL_ERROR;
                    }
                    append_char(our_string, c);
                    present_state = F5;
                    scanner_first_token = false;
                    break;
                } //numbers 1..9
                if(c == 39){
                    if((our_string = init_cstring_size(256)) == NULL){
                        global_error_code = INTERNAL_ERROR;
                        return INTERNAL_ERROR;
                    }
                    present_state = Q2;
                    scanner_first_token = false;
                    break;
                } // ' simpole apostrophe
                if(c == 34){ present_state = Q3; break;} // \" double apostrophe
                if(c == 35){ present_state = Q8; break;} // # number sign (hash)
                //-----------operators-----------
                if(c == 47){
                    present_state = F22;
                    scanner_first_token = false;
                    break;
                } // /
                if(c == 61){
                    present_state = F11;
                    scanner_first_token = false;
                    break;
                } // =
                if(c == 60){
                    present_state = F13;
                    scanner_first_token = false;
                    break;
                } // <
                if(c == 62){
                    present_state = F15;
                    scanner_first_token = false;
                    break;
                } // >
                if(c == 33){
                    present_state = Q9;
                    scanner_first_token = false;
                    break;
                } // !
                /*if(c == 58){ present_state = F18; break;} // :
                if(c == 40){ present_state = F24; break;} // (
                if(c == 41){ present_state = F25; break;} // )
                if(c == 42){ present_state = F10; break;} // *
                if(c == 43){ present_state = F20; break;} // +
                if(c == 45){ present_state = F21; break;} // -
                */
                //united operator processing - last final state eliminated, terminating right from S
                if(c == 58 || (c >= 40 && c <= 43) || c == 45 || c == 44){
                    scanner_first_token = false;
                    if((our_string = init_cstring_size(2)) == NULL){
                        global_error_code = INTERNAL_ERROR;
                        return INTERNAL_ERROR; //wihtout \0
                    }
                    append_char(our_string, c);
                    for(int i=0; i<16; i++){
                        if(compare_string(our_string, op_conv[i].str)){
                            add_simple_data(token, op_conv[i].type);
                            free_cstring(our_string);
                            return 0;
                        }
                    }
                    //the program cannot get here
                }
                //error
                global_error_code = LEXICAL_ANALYSIS_ERROR;
                return LEXICAL_ANALYSIS_ERROR;
            //--------------------------------STATES--------------------------------
            case Q1:
                if(c == 43 || c == 45){
                    append_char(our_string, c);
                    present_state = Q11; //passing initialized our_string
                    break;
                } // +, -
                if(is09num(c)){
                    append_char(our_string, c);
                    present_state = F7; //passing initialized our_string
                    break;
                } //0..9
                //error
                free_cstring(our_string);
                indentStackDestroy(stack);
                global_error_code = LEXICAL_ANALYSIS_ERROR;
                return LEXICAL_ANALYSIS_ERROR;
            case Q2: //raw string processsing - KEEPING ESC CHAR
                if(c == 92){
                    append_char(our_string, c);
                    present_state = Q12;
                    break;
                } // '\'
                if(c == 39){ present_state = F9; break;} // ' 
                if(isPrintChar(c)){
                    append_char(our_string, c);
                    break; //printable chars excl '\', ' remains in Q2// && c != 92 && c != 39
                }
                //error
                free_cstring(our_string);
                indentStackDestroy(stack);
                global_error_code = LEXICAL_ANALYSIS_ERROR;
                return LEXICAL_ANALYSIS_ERROR;
            case Q3:
                if(c == 34){ present_state = Q4; break;} //"
                //error
                indentStackDestroy(stack);
                global_error_code = LEXICAL_ANALYSIS_ERROR;
                return LEXICAL_ANALYSIS_ERROR;
            case Q4:
                if(c == 34){ present_state = Q5; break;} //"
                //error
                indentStackDestroy(stack);
                global_error_code = LEXICAL_ANALYSIS_ERROR;
                return LEXICAL_ANALYSIS_ERROR;
            case Q5:
                if(c == 34){ present_state = Q6; break;} //"
                break;
            case Q6:
                if(c == 34){ present_state = Q7; break;} //"
                present_state = Q5;
                break;
            case Q7:
                if(c == 34){ present_state = F19; break;} //"
                present_state = Q5;
                break;
            case Q8:
                if(c != '\n') break;
                if(scanner_first_token){ present_state = S; break;}
                //if c == '\n' and !first_token
                present_state = F2;
                break;
            case Q9:
                if(c == 61){
                    append_char(our_string, c);
                    present_state = F17;
                    break;
                } //=
                free_cstring(our_string);
                indentStackDestroy(stack);
                global_error_code = LEXICAL_ANALYSIS_ERROR;
                return LEXICAL_ANALYSIS_ERROR;
            case Q10:
                if(is09num(c)){
                    append_char(our_string, c);
                    present_state = F6; //passing initialized our_string
                    break;
                }
                free_cstring(our_string);
                indentStackDestroy(stack);
                global_error_code = LEXICAL_ANALYSIS_ERROR;
                return LEXICAL_ANALYSIS_ERROR;
            case Q11:
                if(is09num(c)){
                    append_char(our_string, c);
                    present_state = F7; //passing initialized our_string
                    break;
                }
                free_cstring(our_string);
                indentStackDestroy(stack);
                global_error_code = LEXICAL_ANALYSIS_ERROR;
                return LEXICAL_ANALYSIS_ERROR;
            case Q12:
                if(isPrintChar(c)){
                    append_char(our_string, c);
                    present_state = Q2;
                    break;
                }
                free_cstring(our_string);
                indentStackDestroy(stack);
                global_error_code = LEXICAL_ANALYSIS_ERROR;
                return LEXICAL_ANALYSIS_ERROR;
            //-------------------------FINAL STATES----------------------------------
            case F1:
                //counting the ws
                if(isSpace(c)){
                    ws++;
                    break;
                }
                //ungetc the first char that is not ws
                ungetc(c, f);

                //empty stack == push INDENT
                if(indentStackEmpty(stack)){
                    indentStackPush(stack, ws);
                    add_simple_data(token, INDENT);
                    ws = 0;
                    scanner_first_token = false;
                    return 0;
                }else{
                    int tmp_cmp_stack_top = indentStackTopCompare(stack, ws);
                    //compare stack top
                    if(tmp_cmp_stack_top == 0){
                        //NOT generating INDENT or DEDENT
                        present_state = S;
                        ws = 0;
                        break;
                    }else if(tmp_cmp_stack_top == 1){
                        if(dedent){
                            //error
                            indentStackDestroy(stack);
                            global_error_code = LEXICAL_ANALYSIS_ERROR;
                            return LEXICAL_ANALYSIS_ERROR;
                        }
                        indentStackPush(stack, ws);
                        add_simple_data(token, INDENT);
                        ws = 0;
                        scanner_first_token = false;
                        return 0;
                    }else if(tmp_cmp_stack_top == -1){
                        indentStackPop(stack);
                        add_simple_data(token, DEDENT);
                        scanner_first_token = false;
                        //ws conent remains the same, in the next iteration
                        //it is doing control of more dedents on the line

                        //when the stack is empty by now, there is no need to check for
                        //another dedent. if ws != 0 that means error
                        if(indentStackEmpty(stack)){
                            if(ws){
                                //error
                                indentStackDestroy(stack);
                                global_error_code = LEXICAL_ANALYSIS_ERROR;
                                return LEXICAL_ANALYSIS_ERROR;
                            }
                            ws = 0;
                            dedent = false;
                            present_state = S;
                            break;
                        }
                        return 0;
                        //TODO check for correctness
                    }
                }
                break;
            case F2:
                //generating token EOL
                ungetc(c, f);
                scanner_first_token = true;
                add_simple_data(token, EOL);
                ++line_count;
                return 0;
            case F4:
                //pre -> our_string != NULL
                if(isIdNameStart(c) || is09num(c)){
                    present_state = F4;
                    append_char(our_string, c); //adding char to string
                    break;
                }else{
                    //not a string char
                    //ending final state
                    ungetc(c, f);
                    //checking for keyword
                    if(isKeyword(our_string) != 999){ //is keyword
                        add_simple_data(token, isKeyword(our_string));
                        free_cstring(our_string);
                        return 0;
                    }
                    add_id(token, our_string);
                    return 0;
                }
                break;
            case F5:
                //pre -> our_string != NULL
                if(is09num(c)){
                    append_char(our_string, c);
                    break;
                }
                if(c == 46){ //.
                    append_char(our_string, c);
                    present_state = Q10; //passing initialized our_string
                    break;
                }
                if(c == 'e' || c == 'E'){
                    append_char(our_string, c);
                    present_state = Q1; //passing initialized our_string
                }
                
                //end of the number
                ungetc(c, f);
                cstrToInt(our_string, token); //check return value!!!
                return 0;
            case F6:
                if(is09num(c)){
                    append_char(our_string, c);
                    break;
                }
                if(c == 'e' || c == 'E'){
                    append_char(our_string, c);
                    present_state = Q1; //passing initialized our_string
                    break;
                }

                //end of the decimal number
                ungetc(c, f);
                cstrToDec(our_string, token);
                return 0;
            case F7:
                if(is09num(c)){
                    append_char(our_string, c);
                    break;
                } 
                
                //end of the decimal number expressed in exponential notation
                ungetc(c, f);
                cstrToDec(our_string, token);
                return 0;
            case F8: //this case NEEDED to be FUCKING TESTED OUT
                //000000000 -> 0
                if(c == 48) break; //'0'
                
                if(is19num(c) || isIdNameStart(c)){
                    //error
                    indentStackDestroy(stack);
                    global_error_code = LEXICAL_ANALYSIS_ERROR;
                    return LEXICAL_ANALYSIS_ERROR;
                }

                ungetc(c, f);
                add_int(token, 0);
                return 0;
            case F9: //end of the string
                ungetc(c, f);
                resize_cstring(our_string, our_string->length+1);
                add_string(token, our_string);
                return 0;
            case F11: //assignment op
                if(c == 61){ //=
                    present_state = F12;
                    break;
                }

                ungetc(c, f);
                add_simple_data(token, ASS); //:nicoSmug
                return 0;
            case F12: //equal op
                ungetc(c, f);
                add_simple_data(token, EQ);
                return 0;
            case F13:
                if(c == 60){ //<
                    present_state = F14;
                    break;
                }
                
                ungetc(c, f);
                add_simple_data(token, L);
                return 0;
            case F14:
                ungetc(c, f);
                add_simple_data(token, LEQ);
                return 0;
            case F15:
                if(c == 62){ //>
                    present_state = F16;
                    break;
                }

                ungetc(c, f);
                add_simple_data(token, G);
                return 0;
            case F16:
                ungetc(c, f);
                add_simple_data(token, GEQ);
                return 0;
            case F17:
                ungetc(c, f);
                add_simple_data(token, NEQ);
                return 0;
            case F19: //case F18 moved to S
                ungetc(c, f);
                add_simple_data(token, DOCS);
                return 0;
            case F22: //cases F20-21 moved to S
                if(c == 47){ // /
                    present_state = F23;
                    break;
                }

                ungetc(c, f);
                add_simple_data(token, SL);
                return 0;
            case F23:
                ungetc(c, f);
                add_simple_data(token, DSL);
                return 0;
            //cases F24-25 moved to S
        }
    }
    global_error_code = LEXICAL_ANALYSIS_ERROR;
    return LEXICAL_ANALYSIS_ERROR; //only for compiling NOT SURE for this line tbh
}

int cstrToInt(cstring *our_string, Token *token){
    int tmp;
    if(sscanf(get_cstring_string(our_string), "%i", &tmp) == EOF){
        global_error_code = INTERNAL_ERROR;
        return INTERNAL_ERROR;
    }
    add_int(token, tmp);
    free_cstring(our_string);
    
    return 0;
}

int cstrToDec(cstring *our_string, Token *token){
    double tmp;
    if(sscanf(get_cstring_string(our_string), "%lf", &tmp) == EOF){
        global_error_code = INTERNAL_ERROR;
        return INTERNAL_ERROR;
    }
    add_dec(token, tmp);
    free_cstring(our_string);
    
    return 0;
}

e_type isKeyword(cstring *string){
    for(int i=0; i < 7; i++){ //7 is the number of keywords specified in ifj19
        if(compare_string(string, keyword_conv[i].str)){
            //the string is a keyword
            return keyword_conv[i].type;
        }
    }
    //the string is not a keyword
    return 999;
}

bool isSpace(char c){
    //ws without new line
    return c == 8 || c == 9 || c == 11 || c == 12 || c == 32; //ascii ws
}
bool isIdNameStart(char c){
    //_a..zA..Z
    //_ 95
    //a 97 z 122
    //A 65 Z 90
    return (c == 95) || (c >= 97 && c <= 122) || (c >= 65 && c <= 90);
}
bool isIdName(char c){
    return isIdNameStart(c) || is09num(c);
}
bool is09num(char c){
    //0 -> 48
    //9 -> 57
    return (c >= 48 && c <= 57);
}
bool is19num(char c){
    return (c >= 49 && c <= 57);
}
bool isPrintChar(char c){
    return (c >= 32 && c <= 126);
}
int checkInDedent(unsigned ws, tIndentStack *stack){
    if(indentStackEmpty(stack)){
        indentStackPush(stack, ws);
    }else if(indentStackTopCompare(stack, ws) == -1){
        indentStackPush(stack, ws);

    }
    return 0; //for compilation only
}

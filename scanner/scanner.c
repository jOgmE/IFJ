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

int getToken(FILE *f, Token *token){
    //checking static variable
    static tIndentStack *stack;
    static unsigned ws = 0; //ws counter
    static bool scanner_first_token = true; //first token being readed
    if(stack == NULL){
        stack = indentStackInit();
    }

    //declaring variables
    int present_state = S; //the actual state
    char resolved = -1; //ends the FSM
    int c = 0; //character to be readed
    bool dedent = false; //true when the last token was dedent

    if(ws){ //ws (!= 0) was not cleared => dedent was the last token
        present_state = F1;
        dedent = true;
    }

    //ends while when resolved == 0
    while(resolved){
        if((c = fgetc(f)) == EOF){
            return 2;
        }
        
        //FSM
        switch(present_state){
            //starting state
            case S:
                if(isSpace(c)){ present_state = scanner_first_token ? F1 : S; ws++; break;} //incrementing ws
                if(c == '\n'){ present_state = scanner_first_token ? S : F2; break;}
                if(isIdNameStart(c)){ present_state = F4; break;}
                if(c == 48){ present_state = F9; break;} // 0
                if(is19num(c)){ present_state = F5; break;}
                if(c == 39){ present_state = Q2; break;} // ' simpole apostrophe
                if(c == 34){ present_state = Q3; break;} // \" double apostrophe
                if(c == 35){ present_state = Q8; break;} // # number sign (hash)
                //operators
                if(c == 42){ present_state = F10; break;} // *
                if(c == 43){ present_state = F20; break;} // +
                if(c == 45){ present_state = F21; break;} // -
                if(c == 47){ present_state = F22; break;} // /
                if(c == 61){ present_state = F11; break;} // =
                if(c == 60){ present_state = F13; break;} // <
                if(c == 62){ present_state = F15; break;} // >
                if(c == 33){ present_state = Q9; break;} // !
                if(c == 58){ present_state = F18; break;} // :
                if(c == 40){ present_state = F24; break;} // (
                if(c == 41){ present_state = F25; break;} // )
                //error
                break;
            //states
            case Q1:
                if(c == 43 || c == 45){ present_state = Q11; break;} // +, -
                if(is09num(c)){ present_state = F7; break;} //0..9
                //error
                break;
            case Q2:
                if(c == 92){ present_state = Q12; break;} // '\'
                if(c == 39){ present_state = F9; break;} // ' 
                if(isPrintChar(c)) break; //printable chars excl '\', ' remains in Q2// && c != 92 && c != 39
                //error
                break;
            case Q3:
                if(c == 34){ present_state = Q4; break;} //"
                //error
                break;
            case Q4:
                if(c == 34){ present_state = Q5; break;} //"
                //error
                break;
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
                if(c == 61){ present_state = F17; break} //=
                return -1;
            case Q10:
                if(is09num(c)){ present_state = F6; break;}
                return -1;
            case Q11:
                if(is09num(c)){ present_state = F7; break;}
                return -1;
            case Q12:
                if(isPrintChar(c)){ present_state = Q2; break;}
                return -1;
            //final states
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
                            return -1; //proper error code missing
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
                                return -1; //proper error code missing
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
                scanner_first_token = true;
                add_simple_data(token, EOL);
                return 0;
            case F3:
                //TODO
                //control for keyword and
                //returning a token with
                //type 'keyword'
                if(isIdNameStart(c) || is09num(c)){
                    present_state = F4;
                    //save the char to the string
                    break;
                }
                //TODO
                break;
        }
    }
}

bool isSpace(char c){
    return (c >= 8 && c <= 13) || (c == 32); //ascii ws
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
}

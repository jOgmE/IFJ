/* @author jogme
 * @date 2019.11.23
 *
 * Test file for scanner.
 */

#include "../../lib/dynamic_string.h"
#include "../../lib/errors.h"
#include "../../scanner/token.h"
#include "../../scanner/scanner.h"

//global variable for file
FILE *f;

convert types[] = {
    {INDENT, "indent"},
    {DEDENT, "dedent"},
    {EOL, "EOL"},
    {DEC, "DEC"},
    {INT, "INT"},
    {ID, "ID"},
    {STR, "STR"},
    {L, "L (<)"},
    {LEQ, "LEQ (<=)"},
    {G, "G (>)"},
    {GEQ, "GEQ (>=)"},
    {EQ, "EQ (==)"},
    {ASS, "ASS (=)"},
    {NEQ, "NEQ (!=)"},
    {PLUS, "PLUS (+)"},
    {MINUS, "MINUS (-)"},
    {AST, "AST (*)"},
    {SL, "SL (/)"},
    {DSL, "DSL (//)"},
    {COL, "COL (:)"},
    {LPA, "LPA '('"},
    {RPA, "RPA ')'"},
    {EOFILE, "EOF"},
    {COM, "COM (,)"},
    {DEF, "def"},
    {ELSE, "else"},
    {IF, "if"},
    {NONE, "None"},
    {PASS, "pass"},
    {RETURN, "return"},
    {WHILE, "while"},
    {DOCS, "DOCS"}
};

const char *getTokenTypeStr(e_type type){
    for(int i=0; i<32; i++){
        if(types[i].type == type) return types[i].str;
    }

    return "NaN";
}

void printToken(Token *token){
    //printf("-------Token-------\n");
    e_type type = getTokenType(token);
    printf("[%s", getTokenTypeStr(type));

    if(type == INT){
        int i;
        getIntValue(token, &i);
        printf(", %i]", i);
    }
    else if(type == DEC){
        double d;
        getDecValue(token, &d);
        printf(", %lf]", d);
    }
    else if(type == STR || type == ID){
        //something
        cstring *s = getTokenStrValue(token);
        printf(", %s]", get_cstring_string(s));
    }
    else{
        printf("]");
    }
}

void printTokenArr(Token **token_arr, size_t size){
    if(token_arr){
        printf("\n--------------------Tokens--------------------\n");
        printf(">Number of tokens: %ld\n\n", size);
        for(size_t i=0; i<size; i++){
            printToken(token_arr[i]);
            if(getTokenType(token_arr[i]) != EOFILE) printf(", ");
            else printf("\n");
            if(getTokenType(token_arr[i]) == EOL) printf("\n"); //10 on one line
        }
        printf("----------------------------------------------\n\n");
    }
}

void printSeparator(const char *sep){
    printf("**************\n");
    printf("   %s\n", sep);
    printf("**************\n");
}

int testTokenReading(){
    size_t sizeOfArr = 124;
    Token **token_arr = initTokenArr(sizeOfArr);
    if(token_arr == NULL){
        return -1;
    }
    size_t i = 0;
    do{
        getToken(f, token_arr[i++]);
        if(global_error_code != SUCCESS){
            //edit to print error message
            //fprintf(stderr, "error happened\n");
            print_internal_error(global_error_code, ERROR, "error happened\n");
            freeTokenArr(token_arr, sizeOfArr);
            return -1; //error
        }
        if(i == sizeOfArr){
            //realloc token_arr
            if((reallocTokenArr(token_arr, sizeOfArr, sizeOfArr*2))){
                //error happened
                return -1;
            }
            sizeOfArr *= 2;
        }
    }while(getTokenType(token_arr[i-1]) != EOFILE);

    printTokenArr(token_arr, i);
    freeTokenArr(token_arr, sizeOfArr);
    return 0;
}

void runTest(const char *test){
    f = fopen(test, "r");
    printSeparator(test);
    testTokenReading();
    fclose(f);
}

int main(){
    //test1
    runTest("test1");
    //test2
    runTest("test2");

    return 0;
}

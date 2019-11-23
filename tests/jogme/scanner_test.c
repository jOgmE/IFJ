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
    else if(type == STR){
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
        printf("--------------------Tokens--------------------\n");
        printf(">Number of tokens: %ld\n\n", size);
        for(size_t i=0; i<size; i++){
            printToken(token_arr[i]);
            printf(", ");
            if(!(i % 10)) printf("\n"); //10 on one line
        }
        printf("----------------------------------------------\n");
    }
}

void printSeparator(const char *sep){
    printf("---------------------------------------------------------\n");
    printf("%30s\n", sep);
    printf("---------------------------------------------------------\n");
}

int testTokenReading(){
    size_t sizeOfArr = 124;
    Token **token_arr = initTokenArr(sizeOfArr);
    size_t i = 0;
    while(getTokenType(token_arr[i]) != EOFILE){//don't fcking know if this works or not
        getToken(f, token_arr[i++]);
        if(global_error_code != SUCCESS){
            //edit to print error message
            fprintf(stderr, "error happened\n");
            //print_internal_error(global_error_code, ERROR, "error happened\n");
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
    }
    printTokenArr(token_arr, i);
    freeTokenArr(token_arr, sizeOfArr);
    return 0;
}

int main(){
    //TEST1
    f = fopen("test1", "r");
    printSeparator("Test 1");
    testTokenReading();
    printSeparator("END Test 1");
    fclose(f);

    return 0;
}

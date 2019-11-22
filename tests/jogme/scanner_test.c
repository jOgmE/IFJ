/* @author jogme
 * @date 2019.11.23
 *
 * Test file for scanner.
 */

#define _libPath_ "../../lib/"
#define _scannerPath_ "../../scanner/"

#include _scannerPath_"scanner.h"
#include _scannerPath_"token.h"
#include _libPath_"dynamic_string.h"

//global variable for file
FILE *f;

void printToken(Token *token){
    printf("-------Token-------\n");
    e_type type = getTokenType(token);
    printf("[%s", type);

    if(type == INT){
        int i;
        getIntValue(token, &i);
        printf(", %i]\n", i);
    }
    else if(type == DEC){
        double d;
        getDecValue(token, &d);
        printf(", %lf]\n", d);
    }
    else if(type == STR){
        //something
        cstring *s;
        getStrValue(token, s);
        printf(", %s]\n", getString(s));
    }
    else{
        printf("]\n");
    }
}

void printSeparator(const char *sep){
    printf("---------------------------------------------------------\n");
    printf("%20s\n", sep);
    printf("---------------------------------------------------------\n");
}

int test1(Token token){
    
}

int main(){
    //TEST1
    f = fopen("test1", "r");
    printSeparator("Test 1");
    test1(token1);

    return 0;
}

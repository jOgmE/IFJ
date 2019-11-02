/* autor: xpocsn00
 * date: 2019-11-92
 *
 * The core file of the scanner. It's implementing the FSM.
 * Characters readed in are converted into tokens.
 */

#include "scanner.h"

/* Function implements the FSM needed to get the token.
 * Returns a token or error.
 */
Token *getToken(FILE *f){
    int present_state = S; //the actual state
    char resolved = -1; //ends the FSM
    int c = 0; //character to be readed

    //ends while when resolved == 0
    while(resolved){
        
    }
}

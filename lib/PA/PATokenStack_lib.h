


// includes go up here ^

typedef struct patokenstackelement PAStackElem;

typedef struct patokenstack PAStack;

void PAInit ( PAStack **s );

void PAPush ( PAStack *s, e_type content, int isTerm );

e_type PAClosestTerm ( PAStack *s );

void PAPop ( PAStack *s );

void PAYeet ( PAStack *s );

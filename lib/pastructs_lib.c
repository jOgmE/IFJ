// Written and directed by: Filip Draber
// Date: 20. 11. 2019

#define TC 10
#define PA_RULES_COUNT 2

// assigns isTerm values to enumerated tokens
extern enum 			{ PA_EXPR, 	PA_VAR,		PA_INT,		PA_PLUS,	PA_EMPTY};
extern int isTokenTerm[TC] =	{ 0,		1,		1,		0, 		0, 		0, 		0, 		0, 		0 };

typedef struct patoken {

	int type; // token enum
	int isTerm;
	char repr;

} PAToken;

typedef struct parule {

	int left;
	int right[3];

} PARule;

extern PARule[PA_RULES_COUNT] = {
	{ PA_EXPR, /* -> */ { PA_EXPR, PA_PLUS, PA_EXPR } },
	{ PA_EXPR, /* -> */ { PA_EXPR, PA_EMPTY, PA_EMPTY } }
};



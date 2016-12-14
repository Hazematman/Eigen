#include <stdio.h>
#include <string.h>
#include "script/scanner.h"
#include "scannerTest.h"
#include "../tassert.h"

#define DEBUG 0

static char *test1 = "func main() { return 42; }";
static enum TokenType test1Types[] = {TOKEN_FUNC, TOKEN_ID, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RETURN, TOKEN_NUMBER, TOKEN_SEMICOLON, TOKEN_RBRACE};
static char *test1Strs[] = {"func", "main", "(", ")", "{", "return", "42", ";", "}"};

static void scannerTest1() {
    struct Array *toks;

    toks = scannerParse(test1);

    tassert(toks != NULL);

#if DEBUG
    for(size_t i=0; i < arrayLength(toks); i++) {
        struct Token *t = arrayGet(toks, i);
        printf("%i %s\n", t->type, t->str);
    }
#endif

    tassert(arrayLength(toks) == 9);

    for(size_t i=0; i < arrayLength(toks); i++) {
        struct Token *t = arrayGet(toks, i);
        tassert(t->type == test1Types[i]);
        tassert(strcmp(t->str, test1Strs[i]) == 0);
    }

    tokenArrayDestroy(toks);
}

void scannerTest() {
    scannerInit();

    scannerTest1();

    scannerDeinit();
}

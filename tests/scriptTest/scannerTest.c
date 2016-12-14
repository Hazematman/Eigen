#include <stdio.h>
#include <string.h>
#include "script/scanner.h"
#include "scannerTest.h"
#include "../tassert.h"

#define DEBUG 0

static char *test1 = "func main() { return 42; }";
static enum TokenType test1Types[] = {TOKEN_FUNC, TOKEN_ID, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RETURN, TOKEN_NUMBER, TOKEN_SEMICOLON, TOKEN_RBRACE};
static char *test1Strs[] = {"func", "main", "(", ")", "{", "return", "42", ";", "}"};
static size_t test1Len = sizeof(test1Types)/sizeof(enum TokenType);

static char *test2 = "37.94343";
static enum TokenType test2Types[] = {TOKEN_NUMBER};
static char *test2Strs[] = {"37.94343"};
static size_t test2Len = sizeof(test2Types)/sizeof(enum TokenType);

static char *test3 = "2.";
static char *test4 = "2.23213a";
static char *test5 = "\"Hello World!!!!! ";
static char *test6 = "\"Hello World\\\"";

static char *test7 = "\"Hello World!\"";
static enum TokenType test7Types[] = {TOKEN_STRING};
static char *test7Strs[] = {"\"Hello World!\""};
static size_t test7Len = sizeof(test7Types)/sizeof(enum TokenType);

static void doTest(char *input, enum TokenType *tokens, char *strs[], size_t len) {
    struct Array *toks;

    toks = scannerParse(input);

    tassert(toks != NULL);

#if DEBUG
    for(size_t i=0; i < arrayLength(toks); i++) {
        struct Token *t = arrayGet(toks, i);
        printf("%i %s\n", t->type, t->str);
    }
#endif

    tassert(arrayLength(toks) == len);

    for(size_t i=0; i < arrayLength(toks); i++) {
        struct Token *t = arrayGet(toks, i);
        tassert(t->type == tokens[i]);
        tassert(strcmp(t->str, strs[i]) == 0);
    }

    tokenArrayDestroy(toks);
}

static void doTestFail(char *input) {
    struct Array *toks;

    toks = scannerParse(input);

    tassert(toks == NULL);
}

void scannerTest() {
    scannerInit();

    doTest(test1, test1Types, test1Strs, test1Len);
    doTest(test2, test2Types, test2Strs, test2Len);

    doTestFail(test3);
    doTestFail(test4);
    doTestFail(test5);
    doTestFail(test6);

    doTest(test7, test7Types, test7Strs, test7Len);

    scannerDeinit();
}

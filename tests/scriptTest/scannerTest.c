#include <stdio.h>
#include <string.h>
#include "script/scanner.h"
#include "scannerTest.h"
#include "../tassert.h"

#define DEBUG 0

#if DEBUG
static int curTest = 0;
#endif

static char *test1 = "func main() { return 42; }";
static enum TokenType test1Types[] = {TOKEN_BOF,TOKEN_FUNC, TOKEN_ID, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RETURN, TOKEN_NUMBER, TOKEN_SEMICOLON, TOKEN_RBRACE, TOKEN_EOF};
static char *test1Strs[] = {"BOF","func", "main", "(", ")", "{", "return", "42", ";", "}","EOF"};
static size_t test1Len = sizeof(test1Types)/sizeof(enum TokenType);

static char *test2 = "37.94343";
static enum TokenType test2Types[] = {TOKEN_BOF,TOKEN_NUMBER,TOKEN_EOF};
static char *test2Strs[] = {"BOF","37.94343","EOF"};
static size_t test2Len = sizeof(test2Types)/sizeof(enum TokenType);

static char *test3 = "2.";
static char *test4 = "2.23213a";
static char *test5 = "\"Hello World!!!!! ";
static char *test6 = "\"Hello World\\\"";

static char *test7 = "\"Hello World!\"";
static enum TokenType test7Types[] = {TOKEN_BOF,TOKEN_STRING,TOKEN_EOF};
static char *test7Strs[] = {"BOF","\"Hello World!\"","EOF"};
static size_t test7Len = sizeof(test7Types)/sizeof(enum TokenType);

static char *test8 = "var array = []";
static enum TokenType test8Types[] = {TOKEN_BOF, TOKEN_VAR, TOKEN_ID, TOKEN_BECOMES, TOKEN_LBRACK, TOKEN_RBRACK, TOKEN_EOF};
static char *test8Strs[] = {"BOF","var", "array", "=", "[", "]","EOF"};
static size_t test8Len = sizeof(test8Types)/sizeof(enum TokenType);

static char *test9 = "var array = [123]";
static enum TokenType test9Types[] = {TOKEN_BOF, TOKEN_VAR, TOKEN_ID, TOKEN_BECOMES, TOKEN_LBRACK, TOKEN_NUMBER, TOKEN_RBRACK, TOKEN_EOF};
static char *test9Strs[] = {"BOF","var", "array", "=", "[","123","]","EOF"};
static size_t test9Len = sizeof(test9Types)/sizeof(enum TokenType);

static char *test10 = "var a = 0; // This is a comment";
static enum TokenType test10Types[] = {TOKEN_BOF, TOKEN_VAR, TOKEN_ID, TOKEN_BECOMES, TOKEN_NUMBER, TOKEN_SEMICOLON, TOKEN_EOF};
static char *test10Strs[] = {"BOF","var", "a", "=", "0", ";","EOF"};
static size_t test10Len = sizeof(test10Types)/sizeof(enum TokenType);

static char *test11 = "// This is just a comment";
static enum TokenType test11Types[] = {TOKEN_BOF, TOKEN_EOF};
static char *test11Strs[] = {"BOF", "EOF"};
static size_t test11Len = sizeof(test11Types)/sizeof(enum TokenType);

static char *test12 = "/* This is a block comment */";
static enum TokenType test12Types[] = {TOKEN_BOF, TOKEN_EOF};
static char *test12Strs[] = {"BOF", "EOF"};
static size_t test12Len = sizeof(test12Types)/sizeof(enum TokenType);

static char *test13 = "/* This is also ***** a block comment **/";
static enum TokenType test13Types[] = {TOKEN_BOF, TOKEN_EOF};
static char *test13Strs[] = {"BOF", "EOF"};
static size_t test13Len = sizeof(test13Types)/sizeof(enum TokenType);

static char *test14 = "/* This should fail";

static char *test15 = "/* this should also fail /";

static char *test16 = "func blah(a,b) {\n return a*b;\n}\n b = bleh(10,2);\n c = true;\n";
static enum TokenType test16Types[] = {TOKEN_BOF, TOKEN_FUNC, TOKEN_ID, TOKEN_LPAREN, TOKEN_ID, TOKEN_COMMA, TOKEN_ID, TOKEN_RPAREN, TOKEN_LBRACE, TOKEN_RETURN, TOKEN_ID, TOKEN_STAR, TOKEN_ID, TOKEN_SEMICOLON, TOKEN_RBRACE, TOKEN_ID, TOKEN_BECOMES, TOKEN_ID, TOKEN_LPAREN, TOKEN_NUMBER, TOKEN_COMMA, TOKEN_NUMBER, TOKEN_RPAREN, TOKEN_SEMICOLON, TOKEN_ID, TOKEN_BECOMES, TOKEN_TRUE, TOKEN_SEMICOLON, TOKEN_EOF};
static char *test16Strs[] = {"BOF","func", "blah", "(", "a", ",", "b", ")", "{", "return", "a", "*", "b", ";", "}", "b", "=", "bleh", "(", "10", ",", "2",")",";","c","=","true",";","EOF"}; 
static size_t test16Len = sizeof(test16Types)/sizeof(enum TokenType);

static void doTest(char *input, enum TokenType *tokens, char *strs[], size_t len) {
    struct Array *toks;

#if DEBUG
    printf("Doing scanner test %d\n", ++curTest);
#endif

    toks = scannerParse(input);

    tassert(toks != NULL);

#if DEBUG
    for(size_t i=0; i < arrayLength(toks); i++) {
        struct Token *t = arrayGet(toks, i);
        printf("\t%i %s\n", t->type, t->str);
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

#if DEBUG
    printf("Doing scanner test %d\n", ++curTest);
#endif

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
    doTest(test8, test8Types, test8Strs, test8Len);
    doTest(test9, test9Types, test9Strs, test9Len);
    doTest(test10, test10Types, test10Strs, test10Len);
    doTest(test11, test11Types, test11Strs, test11Len);
    doTest(test12, test12Types, test12Strs, test12Len);
    doTest(test13, test13Types, test13Strs, test13Len);

    doTestFail(test14);
    doTestFail(test15);

    doTest(test16, test16Types, test16Strs, test16Len);

    scannerDeinit();
}

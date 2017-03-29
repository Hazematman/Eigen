#include <stdio.h>
#include "parserTest.h"
#include "script/scanner.h"
#include "script/parser.h"
#include "../tassert.h"

static char *test1 = "func main() { return 42; }";

void doTest(char *input) {
    struct Array *toks;
    struct TreeNode *ast;

    toks = scannerParse(input);
    for(size_t i=0; i < arrayLength(toks); i++) {
        struct Token *t = arrayGet(toks, i);
        printf("\t%i %s\n", t->type, t->str);
    }
    ast = parserParse(toks);

    size_t elem = *(size_t *)nodeGetElem(ast);
    printf("elem is %lu\n", elem);
}

void parserTest() {
    scannerInit();
    parserInit();

    doTest(test1);

    scannerDeinit();
    parserDeinit();
}

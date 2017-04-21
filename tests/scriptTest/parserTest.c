#include <stdio.h>
#include <stdbool.h>
#include "parserTest.h"
#include "script/scanner.h"
#include "script/parser.h"
#include "../tassert.h"

static char *test1 = "func main() { return 42; }";
static int test1Res[] = {38};
size_t test1Len = sizeof(test1Res)/sizeof(int);

bool compareTree(struct TreeNode *ast, int *tree, size_t nElem, size_t *ind) {
    if(*ind >= nElem) {
        return false;
    }
    struct Token t = *(struct Token*)nodeGetElem(ast);
    bool result = t.type == tree[*ind++];
    for(size_t i=0; i < nodeNumChild(ast); i++) {
        struct TreeNode *child = nodeGetChild(ast, i);
        result &= compareTree(child, tree, nElem, ind);
    }
    return result;
}

void printTree(struct TreeNode *ast, int ii) {
    struct Token t = *(struct Token*)nodeGetElem(ast);
    if(t.type >= MAX_TOKEN_VAL) {
        printf("%d %s\n", ii, parserRules[t.type - MAX_TOKEN_VAL]);
    } else {
        printf("%d %d %s\n", ii, t.type, t.str);
    }

    for(int i=nodeNumChild(ast)-1; i >= 0; i--) {
        struct TreeNode *child = nodeGetChild(ast, i);
        printTree(child, ii+1);
    }
}

void doTest(char *input, int *res, size_t nElem) {
    struct Array *toks;
    struct TreeNode *ast;
    size_t ind = 0;

    toks = scannerParse(input);
    ast = parserParse(toks);

    printTree(ast, 0);
    tassert(compareTree(ast, res, nElem, &ind));
}

void parserTest() {
    scannerInit();
    parserInit();

    doTest(test1, test1Res, test1Len);

    scannerDeinit();
    parserDeinit();
}

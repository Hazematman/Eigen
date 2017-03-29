#include "parser.h"
#include "lr1.h"

static struct Cfg *esCfg = NULL;

void parserInit() {
    esCfg = lr1Create();
}

void parserDeinit() {
    cfgDestroy(esCfg);
}

struct TreeNode *parserParse(struct Array *tokens) {
    struct TreeNode *ast = NULL;

    ast = cfgParse(esCfg, tokens);

    return ast;
}

#include "parser.h"
#include "lr1.h"

static struct Cfg *esCfg = NULL;

char *parserRules[] = {
    "S BOF lines EOF",
    "lines e",
    "lines lines function",
    "lines lines statement",
    "function FUNC ID LPAREN params RPAREN LBRACE statements RBRACE",
    "params e",
    "params paramlist",
    "paramlist ID",
    "paramlist ID COMMA paramlist",
    "statements e",
    "statements statements statement",
    "statement lvalue BECOMES expr SEMICOLON",
    "statement functioncall SEMICOLON",
    "statement IF LPAREN test RPAREN LBRACE statements RBRACE elseclause",
    "statement WHILE LPAREN tests RPAREN LBRACE statements RBRACE",
    "statement FOR LPAREN lvalue BECOMES expr SEMICOLON tests SEMICOLON lvalue BECOMES expr RPAREN LBRACE statements RBRACE",
    "statement RETURN expr SEMICOLON",
    "elseclause e",
    "elseclause ELSE IF LPAREN test RPAREN LBRACE statements RBRACE elseclause",
    "elseclause ELSE LBRACE statements RBRACE",
    "tests test",
    "tests tests AND test",
    "tests tests OR test",
    "test expr EQUAL expr",
    "test expr NEQUAL expr",
    "test expr LTHAN expr",
    "test expr LETHAN expr",
    "test expr GTHAN expr",
    "test expr GETHAN expr",
    "expr term",
    "expr expr PLUS term",
    "expr expr MINUS term",
    "term factor",
    "term term STAR factor",
    "term term SLASH factor",
    "term term PERCENT factor",
    "factor ID",
    "factor NUMBER",
    "factor STRING",
    "factor TRUE",
    "factor FALSE",
    "factor LPAREN expr RPAREN",
    "factor ID LBRACK expr RBRACK",
    "factor functioncall",
    "functioncall ID LPAREN RPAREN",
    "functioncall ID LPAREN arglist RPAREN",
    "arglist expr",
    "arglist expr COMMA arglist",
    "lvalue ID",
    "lvalue ID LBRACK expr RBRACK",
};

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

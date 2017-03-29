#ifndef PARSER_H
#define PARSER_H
#include "util/array.h"
#include "util/ntree.h"

void parserInit();
void parserDeinit();
struct TreeNode *parserParse(struct Array *tokens);

#endif

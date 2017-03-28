#ifndef CFG_H
#define CFG_H
#include "util/array.h"
#include "util/ntree.h"
#include "scanner.h"

enum StateType {
    NONE,
    REDUCE,
    SHIFT
};

struct Cfg;

struct Cfg *cfgCreate(size_t numStates);
void cfgDestroy(struct Cfg *cfg);
void cfgAddTransition(struct Cfg *cfg, size_t state, enum TokenType token, enum StateType type, size_t stateInd);
void cfgAddRule(struct Cfg *cfg, size_t symbol, size_t ruleLen);

struct TreeNode *cfgParse(struct Cfg *cfg, struct Array *tokens);

#endif

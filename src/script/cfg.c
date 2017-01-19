#include <stdlib.h>
#include "util/dfa.h"
#include "cfg.h"
#include "error.h"

struct Cfg {
    struct Array *states;
    struct Array *rules;
};
            
struct Rule {
    size_t len;
    size_t symbol;
};

struct Transition {
    enum TokenType token;
    enum StateType type;
    size_t state;
};

static struct Transition *transition(struct Cfg *cfg, size_t state, enum TokenType token) {
    struct Array *s = *(struct Array **)arrayGet(cfg->states, state);

    /* Find transition in transition array */
    for(size_t i=0; i < arrayLength(s); i++) {
        struct Transition *t = *(struct Transition **)arrayGet(s, i); 
        if(t->token == token) {
            return t;
        }
    }

    return NULL;
}

struct Cfg *cfgCreate(size_t numStates) {
    struct Cfg *cfg = malloc(sizeof(struct Cfg));

    cfg->rules = arrayCreate(sizeof(struct Rule));
    cfg->states = arrayCreate(sizeof(struct Array *));
    for(size_t i=0; i < numStates; i++) {
        struct Array *array = arrayCreate(sizeof(struct Transition));
        arrayPush(cfg->states, &array);
    }

    return cfg;
}

void cfgDestroy(struct Cfg *cfg) {
    for(size_t i=0; i < arrayLength(cfg->states); i++) {
        struct Array *array = *(struct Array **)arrayGet(cfg->states, i);
        arrayDestroy(array);
    }

    arrayDestroy(cfg->states);
    arrayDestroy(cfg->rules);
    free(cfg);
}

void cfgAddTransition(struct Cfg *cfg, size_t state, enum TokenType token, enum StateType type, size_t stateInd) {
    struct Array *s = arrayGet(cfg->states, state);
    struct Transition t = {token, type, stateInd};

    arrayPush(s, &t);
}

void cfgAddRule(struct Cfg *cfg, size_t symbol, size_t ruleLen) {
    struct Rule r = {ruleLen, symbol};
    arrayPush(cfg->rules, &r);
}

void cfgParse(struct Cfg *cfg, struct Array *tokens) {
    struct Array *inputStack = arrayCreate(sizeof(struct Token));
    struct Array *stateStack = arrayCreate(sizeof(size_t));
    size_t state = 0;
    
    arrayPush(stateStack, &state);

    for(size_t i=0; i < arrayLength(tokens); i++) {
        struct Token *t = arrayGet(tokens, i);
        size_t topState = *(size_t *)arrayGet(stateStack, arrayLength(stateStack) - 1);
        struct Transition *newState = transition(cfg, topState, t->type);
        if(newState == NULL) {
            logError("Reached a null state");
            return;
        } else if(newState->type == REDUCE) {
            i--;
            struct Rule *rule = (struct Rule *)arrayGet(cfg->rules, newState->state);

            for(size_t i=0; i < rule->len; i++) {
                arrayPop(inputStack, NULL);
                arrayPop(stateStack, NULL);
            }


            topState = *(size_t *)arrayGet(stateStack, arrayLength(stateStack) - 1);
            newState = transition(cfg, topState, rule->symbol);
            if(newState == NULL) {
                logError("Reached a null state");
                return;
            }
        } else {
            arrayPush(inputStack, &t);
            arrayPush(stateStack, &newState->state);
        }
    }


    arrayDestroy(inputStack);
    arrayDestroy(stateStack);
}

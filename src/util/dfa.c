#include <stdint.h>
#include <string.h>
#include "dfa.h"
#include "array.h"
#include "../error.h"

struct Dfa {
    size_t tokenSize;
    size_t mapSize;
    bool end;
    struct Array *map;
};

struct Dfa *dfaCreate(size_t tokenSize) {
    struct Dfa *dfa = malloc(sizeof(struct Dfa));
    if(dfa == NULL) {
        logError("Out of memory!");
    }

    dfa->tokenSize = tokenSize;
    dfa->mapSize = tokenSize + sizeof(struct Dfa *);
    dfa->end = false;
    dfa->map = arrayCreate(dfa->mapSize);

    return dfa;
}

void dfaDestroy(struct Dfa *dfa) {
    arrayDestroy(dfa->map);
    free(dfa);
}

void dfaSetEnd(struct Dfa *dfa, bool end) {
    dfa->end = end; 
}

bool dfaGetEnd(struct Dfa *dfa) {
    return dfa->end;
}

void dfaAddTransition(struct Dfa *dfa, void *token, struct Dfa *state) {
    uint8_t tempData[dfa->mapSize];
    memcpy(tempData, token, dfa->tokenSize);
    memcpy(tempData+dfa->tokenSize, &state, sizeof(struct Dfa*));
    arrayPush(dfa->map, tempData);
}

struct Dfa *dfaTransition(struct Dfa *dfa, void *token) {
    struct Dfa *out = NULL;
    size_t len = arrayLength(dfa->map);

    for(size_t i=0; i < len; i++) {
        void *trans = arrayGet(dfa->map, i);
        if(memcmp(trans, token, dfa->tokenSize) == 0) {
            memcpy(&out, trans+dfa->tokenSize, sizeof(struct Dfa*));
            return out;
        }
    }

    return out;
}

#include <stdio.h>
#include "util/dfa.h"
#include "dfaTest.h"
#include "../tassert.h"

static struct Dfa *state1;
static struct Dfa *state2;
static struct Dfa *state3;
static struct Dfa *state4;

static int tok1 = 1;
static int tok2 = 2;
static int tok3 = 3;
static int tok4 = 4;
static int tok5 = 5;

static void initStates() {
    state1 = dfaCreate(sizeof(int));
    tassert(state1 != NULL);

    state2 = dfaCreate(sizeof(int));
    tassert(state2 != NULL);

    state3 = dfaCreate(sizeof(int));
    tassert(state3 != NULL);

    state4 = dfaCreate(sizeof(int));
    tassert(state4 != NULL);

    dfaSetEnd(state4, true);

    dfaAddTransition(state1, &tok1, state2);
    dfaAddTransition(state2, &tok1, state1);
    dfaAddTransition(state2, &tok2, state3);
    dfaAddTransition(state3, &tok3, state4);
    dfaAddTransition(state4, &tok4, state4);
    dfaAddTransition(state3, &tok5, state1);
    dfaAddTransition(state2, &tok5, state4);

    tassert(dfaGetEnd(state1) == false);
    tassert(dfaGetEnd(state2) == false);
    tassert(dfaGetEnd(state3) == false);
    tassert(dfaGetEnd(state4) == true);

    //printf("States are 1:%p 2:%p 3:%p 4:%p\n", state1, state2, state3, state4);
}

static void transition() {
    struct Dfa *state = NULL;

    state = dfaTransition(state1, &tok1);
    tassert(state == state2);

    state = dfaTransition(state1, &tok2);
    tassert(state == NULL);

    state = dfaTransition(state2, &tok1);
    tassert(state == state1);

    state = dfaTransition(state2, &tok2);
    tassert(state == state3);

    state = dfaTransition(state3, &tok3);
    tassert(state == state4);

    state = dfaTransition(state4, &tok4);
    tassert(state == state4);
}

static void cleanUp() {
    dfaDestroy(state1);
    dfaDestroy(state2);
    dfaDestroy(state3);
    dfaDestroy(state4);
}

void dfaTest() {
    initStates();
    transition();
    cleanUp();
}

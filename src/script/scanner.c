#include "util/dfa.h"
#include "scanner.h"

static struct Dfa *stateStart;
static struct Dfa *stateWhitespace;
static struct Dfa *stateComment;
static struct Dfa *stateNot;
static struct Dfa *stateError;
static struct Dfa *stateLPAREN;
static struct Dfa *stateRPAREN;
static struct Dfa *stateLBRACE;
static struct Dfa *stateRBRACE;
static struct Dfa *stateLBRACK;
static struct Dfa *stateRBRACK;
static struct Dfa *stateEQUAL;
static struct Dfa *stateNEQUAL;
static struct Dfa *stateLTHAN;
static struct Dfa *stateLETHAN;
static struct Dfa *stateGTHAN;
static struct Dfa *stateGETHAN;
static struct Dfa *statePLUS;
static struct Dfa *stateMINUS;
static struct Dfa *stateSTAR;
static struct Dfa *stateSLASH;
static struct Dfa *statePERCENT;
static struct Dfa *stateAND;
static struct Dfa *stateOR;
static struct Dfa *stateCOMMA;
static struct Dfa *stateSEMICOLON;
static struct Dfa *stateID;
static struct Dfa *stateNUMBER;
static struct Dfa *stateSTRING;

struct Token tokenCreate(enum TokenType type, char *str) {
    struct Token t;
    t.type = type;
    t.str = str;
    return t;
}

void scannerInit() {
    stateStart = dfaCreate(sizeof(char));
    stateWhitespace = dfaCreate(sizeof(char));
    stateComment = dfaCreate(sizeof(char));
    stateNot = dfaCreate(sizeof(char));
    stateError = dfaCreate(sizeof(char));
    stateLPAREN = dfaCreate(sizeof(char));
    stateRPAREN = dfaCreate(sizeof(char));
    stateLBRACE = dfaCreate(sizeof(char));
    stateRBRACE = dfaCreate(sizeof(char));
    stateLBRACK = dfaCreate(sizeof(char));
    stateRBRACK = dfaCreate(sizeof(char));
    stateEQUAL = dfaCreate(sizeof(char));
    stateNEQUAL = dfaCreate(sizeof(char));
    stateLTHAN = dfaCreate(sizeof(char));
    stateLETHAN = dfaCreate(sizeof(char));
    stateGTHAN = dfaCreate(sizeof(char));
    stateGETHAN = dfaCreate(sizeof(char));
    statePLUS = dfaCreate(sizeof(char));
    stateMINUS = dfaCreate(sizeof(char));
    stateSTAR = dfaCreate(sizeof(char));
    stateSLASH = dfaCreate(sizeof(char));
    statePERCENT = dfaCreate(sizeof(char));
    stateAND = dfaCreate(sizeof(char));
    stateOR = dfaCreate(sizeof(char));
    stateCOMMA = dfaCreate(sizeof(char));
    stateSEMICOLON = dfaCreate(sizeof(char));
    stateID = dfaCreate(sizeof(char));
    stateNUMBER = dfaCreate(sizeof(char));
    stateSTRING = dfaCreate(sizeof(char));
}

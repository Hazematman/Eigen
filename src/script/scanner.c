#include <stdio.h>
#include <string.h>
#include "util/dfa.h"
#include "scanner.h"

enum stateNames {
    STATE_start,
    STATE_whitespace,
    STATE_comment,
    STATE_not,
    STATE_error,
    STATE_amp,
    STATE_pipe,
    STATE_LPAREN,
    STATE_RPAREN,
    STATE_LBRACE,
    STATE_RBRACE,
    STATE_LBRACK,
    STATE_RBRACK,
    STATE_EQUAL,
    STATE_NEQUAL,
    STATE_LTHAN,
    STATE_LETHAN,
    STATE_GTHAN,
    STATE_GETHAN,
    STATE_PLUS,
    STATE_MINUS,
    STATE_STAR,
    STATE_SLASH,
    STATE_PERCENT,
    STATE_AND,
    STATE_OR,
    STATE_COMMA,
    STATE_SEMICOLON,
    STATE_ID,
    STATE_NUMBER,
    STATE_STRING,
    STATE_BECOMES,

    NUM_STATES
};

static struct Dfa *states[NUM_STATES];

static char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; 
static char *alphabetNums = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"; 
static char *nums ="0123456789";
static char *whitespace = "\t\n\r "; 
static char lparen = '(';
static char rparen = ')';
static char lbrace = '{';
static char rbrace = '}';
static char equal = '=';
static char lthan = '<';
static char gthan = '>';
static char plus = '+';
static char minus = '-';
static char star = '*';
static char slash = '/';
static char percent = '%';
static char comma = ',';
static char semi = ';';
static char lbrack = '[';
static char rbrack = ']';
static char amp = '&';
static char pipe = '|';
static char not = '!';

static enum TokenType getStateType(struct Dfa *state) {
    switch(&state - &states[0]) {
        case STATE_LPAREN:
            return TOKEN_LPAREN;
        case STATE_RPAREN:
            return TOKEN_RPAREN;
        case STATE_LBRACE:
            return TOKEN_LBRACE;
        case STATE_RBRACE:
            return TOKEN_RBRACE;
        case STATE_LBRACK:
            return TOKEN_LBRACK;
        case STATE_RBRACK:
            return TOKEN_RBRACK;
        case STATE_EQUAL:
            return TOKEN_EQUAL;
        case STATE_NEQUAL:
            return TOKEN_NEQUAL;
        case STATE_LTHAN:
            return TOKEN_LTHAN;
        case STATE_LETHAN:
            return TOKEN_LETHAN;
        case STATE_GTHAN:
            return TOKEN_GTHAN;
        case STATE_GETHAN:
            return TOKEN_GETHAN;
        case STATE_PLUS:
            return TOKEN_PLUS;
        case STATE_MINUS:
            return TOKEN_MINUS;
        case STATE_STAR:
            return TOKEN_STAR;
        case STATE_SLASH:
            return TOKEN_SLASH;
        case STATE_PERCENT:
            return TOKEN_PERCENT;
        case STATE_AND:
            return TOKEN_AND;
        case STATE_OR:
            return TOKEN_OR;
        case STATE_COMMA:
            return TOKEN_COMMA;
        case STATE_SEMICOLON:
            return TOKEN_SEMICOLON;
        case STATE_ID:
            return TOKEN_ID;
        case STATE_NUMBER:
            return TOKEN_NUMBER;
        case STATE_STRING:
            return TOKEN_STRING;
        case STATE_BECOMES:
            return TOKEN_BECOMES;
        default:
            return TOKEN_INVALID;
    }
}

static char *strdupLen(char *test, size_t start, size_t end) {
    size_t len = end - start + 2;
    char *out = malloc(sizeof(char)*len);

    for(size_t i=start; i <= end; i++) {
        out[i-start] = test[start];
    }

    out[len-1] = '\0';

    return out;
}

struct Token tokenCreate(enum TokenType type, char *str) {
    struct Token t;
    t.type = type;
    t.str = str;
    return t;
}

void scannerInit() {
    for(size_t i=0; i < NUM_STATES; i++) {
        states[i] = dfaCreate(sizeof(char));
    }

    dfaSetEnd(states[STATE_ID], true);
    dfaSetEnd(states[STATE_NUMBER], true);
    dfaSetEnd(states[STATE_LPAREN], true);
    dfaSetEnd(states[STATE_RPAREN], true);
    dfaSetEnd(states[STATE_LBRACE], true);
    dfaSetEnd(states[STATE_RBRACE], true);
    dfaSetEnd(states[STATE_BECOMES], true);
    dfaSetEnd(states[STATE_EQUAL], true);
    dfaSetEnd(states[STATE_NEQUAL], true);
    dfaSetEnd(states[STATE_LTHAN], true);
    dfaSetEnd(states[STATE_GTHAN], true);
    dfaSetEnd(states[STATE_LETHAN], true);
    dfaSetEnd(states[STATE_GETHAN], true);
    dfaSetEnd(states[STATE_PLUS], true);
    dfaSetEnd(states[STATE_MINUS], true);
    dfaSetEnd(states[STATE_STAR], true);
    dfaSetEnd(states[STATE_SLASH], true);
    dfaSetEnd(states[STATE_PERCENT], true);
    dfaSetEnd(states[STATE_SEMICOLON], true);
    dfaSetEnd(states[STATE_COMMA], true);
    dfaSetEnd(states[STATE_LBRACK], true);
    dfaSetEnd(states[STATE_RBRACK], true);
    dfaSetEnd(states[STATE_AND], true);
    dfaSetEnd(states[STATE_OR], true);
    dfaSetEnd(states[STATE_whitespace], true);

    size_t numLen = strlen(nums);
    size_t alphabetLen = strlen(alphabet);
    size_t alphabetNumsLen = strlen(alphabetNums);
    size_t whitespaceLen = strlen(whitespace);

    dfaAddTransition(states[STATE_start], &lparen, states[STATE_LPAREN]);
    dfaAddTransition(states[STATE_start], &rparen, states[STATE_RPAREN]);
    dfaAddTransition(states[STATE_start], &lbrace, states[STATE_LBRACE]);
    dfaAddTransition(states[STATE_start], &rbrace, states[STATE_RBRACE]);
    dfaAddTransition(states[STATE_start], &equal, states[STATE_BECOMES]);
    dfaAddTransition(states[STATE_start], &lthan, states[STATE_LTHAN]);
    dfaAddTransition(states[STATE_start], &gthan, states[STATE_GTHAN]);
    dfaAddTransition(states[STATE_start], &plus, states[STATE_PLUS]);
    dfaAddTransition(states[STATE_start], &minus, states[STATE_MINUS]);
    dfaAddTransition(states[STATE_start], &star, states[STATE_STAR]);
    dfaAddTransition(states[STATE_start], &slash, states[STATE_SLASH]);
    dfaAddTransition(states[STATE_start], &percent, states[STATE_PERCENT]);
    dfaAddTransition(states[STATE_start], &comma, states[STATE_COMMA]);
    dfaAddTransition(states[STATE_start], &semi, states[STATE_SEMICOLON]);
    dfaAddTransition(states[STATE_start], &lbrack, states[STATE_LBRACK]);
    dfaAddTransition(states[STATE_start], &rbrack, states[STATE_RBRACK]);
    dfaAddTransition(states[STATE_start], &amp, states[STATE_amp]);
    dfaAddTransition(states[STATE_start], &pipe, states[STATE_pipe]);
    dfaAddTransition(states[STATE_start], &not, states[STATE_not]);
    dfaAddListTransitions(states[STATE_start], nums, numLen, states[STATE_NUMBER]);
    dfaAddListTransitions(states[STATE_start], alphabet, alphabetLen, states[STATE_ID]);
    dfaAddListTransitions(states[STATE_start], whitespace, whitespaceLen, states[STATE_whitespace]);

    dfaAddListTransitions(states[STATE_ID], alphabetNums, alphabetNumsLen, states[STATE_ID]);

    dfaAddListTransitions(states[STATE_NUMBER], nums, numLen, states[STATE_NUMBER]);
    dfaAddListTransitions(states[STATE_NUMBER], alphabet, alphabetLen, states[STATE_error]);

    dfaAddTransition(states[STATE_BECOMES], &equal, states[STATE_EQUAL]);

    dfaAddTransition(states[STATE_not], &equal, states[STATE_NEQUAL]);

    dfaAddTransition(states[STATE_LTHAN], &equal, states[STATE_LETHAN]);

    dfaAddTransition(states[STATE_GTHAN], &equal, states[STATE_GETHAN]);

    dfaAddTransition(states[STATE_SLASH], &slash, states[STATE_comment]);

    dfaAddListTransitions(states[STATE_whitespace], whitespace, whitespaceLen, states[STATE_whitespace]);
}

struct Array *scannerParse(char *text) {
    // TODO need to think of a way to do propper error handling and propagation
    struct Array *out = arrayCreate(sizeof(struct Token));
    struct Dfa *state = states[STATE_start];
    size_t lineLen = strlen(text);
    size_t start = 0;
    for(size_t i=0; i < lineLen; i++) {
        struct Dfa *newState = dfaTransition(state, &text[i]);
        if(newState == NULL) {
            if(dfaGetEnd(state) == false) {
                printf("Error parsing token %c on state %p\n", text[i], state);
                return out;
            }
            if(state != states[STATE_whitespace] && state != states[STATE_comment]) {
                struct Token t = tokenCreate(getStateType(state), strdupLen(text, start, i));
                arrayPush(out, &t);
            }

            state = states[STATE_start];
            i--;
            start = i;
        } else {
            state = newState;
        }
    }


    return out;
}

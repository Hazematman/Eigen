#ifndef DFA_H
#define DFA_H
#include <stdlib.h>
#include <stdbool.h>

struct Dfa;

struct Dfa *dfaCreate(size_t tokenSize);
void dfaDestroy(struct Dfa *dfa);
void dfaSetEnd(struct Dfa *dfa, bool end);
bool dfaGetEnd(struct Dfa *dfa);
void dfaAddTransition(struct Dfa *dfa, void *token, struct Dfa *state);
struct Dfa *dfaTransition(struct Dfa *dfa, void *token);

#endif

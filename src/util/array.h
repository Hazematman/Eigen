#ifndef ARRAY_H
#define ARRAY_H
#include <stdlib.h>

struct Array;

struct Array *arrayCreate(size_t eSize);
void arrayDestroy(struct Array *ary);
size_t arrayLength(struct Array *ary);
void arrayPush(struct Array *ary, void *elem);
void *arrayGet(struct Array *ary, size_t index);

#endif

#ifndef ARRAY_H
#define ARRAY_H
#include <stdlib.h>

struct Array;

/* Generic resizeable array object
 *
 * Can contain an arbitrary amount of elements that
 * are all the same type.
 *
 * Notes:
 */

struct Array *arrayCreate(size_t eSize);
void arrayDestroy(struct Array *ary);
size_t arrayLength(struct Array *ary);
void arrayPush(struct Array *ary, void *elem);
void arrayPop(struct Array *ary, void *store);
void arrayDelete(struct Array *ary, size_t index);
void *arrayGet(struct Array *ary, size_t index);
void *arrayGetData(struct Array *ary);

#endif

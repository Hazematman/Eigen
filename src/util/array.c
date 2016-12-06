#include <stdint.h>
#include <string.h>
#include "array.h"
#include "../error.h"

#define START_SIZE 16

struct Array {
    size_t elemSize;
    size_t length;
    size_t cap;
    uint8_t *data;
};

struct Array *arrayCreate(size_t eSize) {
    struct Array *ary = malloc(sizeof(struct Array));
    if(ary == NULL) {
        logError("Out of memory!");
    }

    ary->elemSize = eSize;
    ary->cap = START_SIZE;
    ary->length = 0;

    ary->data = malloc(sizeof(uint8_t)*ary->elemSize);
    if(ary->data == NULL) {
        logError("Out of memory!");
    }

    return ary;
}

void arrayDestroy(struct Array *ary) {
    free(ary->data);
    free(ary);
}

size_t arrayLength(struct Array *ary) {
    return ary->length;
}

void arrayPush(struct Array *ary, void *elem) {
    if(ary->length == ary->cap) {
        ary->cap *= 2;
        ary->data = realloc(ary->data, sizeof(uint8_t)*ary->cap);
        if(ary->data == NULL) {
            logError("Out of Memory!");
        }
    }

    memcpy(&ary->data[ary->elemSize*ary->length], elem, ary->elemSize);
    ary->length++;
}

void *arrayGet(struct Array *ary, size_t index) {
    return &ary->data[ary->elemSize*index];
}

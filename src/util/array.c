#include <stdio.h>
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

    ary->data = malloc(sizeof(uint8_t)*ary->elemSize*START_SIZE);
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
        ary->data = realloc(ary->data, sizeof(uint8_t)*ary->elemSize*ary->cap);
        if(ary->data == NULL) {
            logError("Out of Memory!");
        }
    }

    memcpy(&ary->data[ary->elemSize*ary->length], elem, ary->elemSize);
    ary->length++;
}

void arrayPop(struct Array *ary, void *store) {
    if(ary->length == 0) {
        logError("Can't pop off element from empty array");
    }
    ary->length--;

    if(store != NULL) {
        memcpy(store, &ary->data[ary->elemSize*ary->length], ary->elemSize);
    }
}

void arrayDelete(struct Array *ary, size_t index) {
    if(ary->length == 0) {
        logError("Cant delete elements from empty array");
    }

    for(size_t i=index+1; i <= ary->length; i++) {
        memcpy(&ary->data[ary->elemSize*(i-1)], &ary->data[ary->elemSize*i], ary->elemSize);
    }

    ary->length--;
}

void *arrayGet(struct Array *ary, size_t index) {
    return &ary->data[ary->elemSize*index];
}

void *arrayGetData(struct Array *ary) {
    return ary->data;
}

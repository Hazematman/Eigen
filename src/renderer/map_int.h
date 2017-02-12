#ifndef MAP_INT_H
#define MAP_INT_H
#include "chunk.h"

struct Map {
    int width;
    int height;
    int numXChunk;
    int numYChunk;
    struct Array *chunks;
};

#endif

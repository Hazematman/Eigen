#ifndef MAP_INT_H
#define MAP_INT_H
#include "chunk.h"

/* Internal header with structure definitions
 * for render module.
 */

struct Texture {
    GLuint id;
    int width, height;
};

struct Map {
    int width;
    int height;
    int numXChunk;
    int numYChunk;
    struct Array *chunks;
};

struct Sprite {
    struct Texture *tex; 
    float x,y,radius;
};

#endif

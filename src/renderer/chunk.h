#ifndef CHUNK_H
#define CHUNK_H
#include "gl_core.h"

#define CHUNK_SIZE 16

struct Tile {
    uint16_t type;
    float radius;
};

struct Wall {
    uint16_t type;
    float lower;
    float higher;
};

struct ChunkVert {
    float x,y,z;
    float u,v;
    float r,g,b,a;
};

struct Chunk {
    GLuint vbo;
    GLuint vao;
};

void chunkCreate(struct Chunk *chunk, struct Tile *tiles, struct Wall *walls, size_t numWalls);

#endif

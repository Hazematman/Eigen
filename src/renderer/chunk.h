#ifndef CHUNK_H
#define CHUNK_H
#include "gl_core.h"

#define CHUNK_SIZE 16
#define TILE_SIZE 32

struct Tile {
    uint32_t type;
    float radius;
};

struct Wall {
    uint32_t type;
    float lower;
    float higher;
    int32_t x;
    int32_t y;
};

struct ChunkVert {
    float x,y,z;
    float u,v;
    float r,g,b,a;
};

struct Chunk {
    GLuint vbo;
    GLuint vao;
    size_t numVerts;
};

void chunkCreate(struct Chunk *chunk, struct Tile *tiles, struct Wall *walls, size_t numWalls);

#endif

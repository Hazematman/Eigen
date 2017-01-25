#ifndef CHUNK_H
#define CHUNK_H
#include "gl_core.h"

struct ChunkVert {
    float x,y,z;
    float u,v;
    float r,g,b,a;
};

struct Chunk {
    GLuint vbo;
    GLuint vao;
};

#endif

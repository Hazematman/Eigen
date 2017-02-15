#include "chunk.h"
#include "renderer.h"
#include "util/array.h"

static void getTexLoc(uint16_t id, struct ChunkVert *p1, struct ChunkVert *p2, struct ChunkVert *p3, struct ChunkVert *p4, struct ChunkVert *p5, struct ChunkVert *p6) {
    // Hardcoded for astrid's tiles right now 8x5 32x32 tiles
    int tId = id - 1;
    int left = tId % 8;
    int top = tId / 8;

    p1->u = (left*32) / 256.0f;
    p1->v = (top*32) / 160.0f;
    p1->r = 1.0; p1->g = 1.0; p1->b = 1.0; p1->a = 1.0;

    p2->u = (left*32+32) / 256.0f;
    p2->v = (top*32) / 160.0f;
    p2->r = 1.0; p2->g = 1.0; p2->b = 1.0; p2->a = 1.0;

    p3->u = (left*32+32) / 256.0f;
    p3->v = (top*32+32) / 160.0f;
    p3->r = 1.0; p3->g = 1.0; p3->b = 1.0; p3->a = 1.0;

    p4->u = (left*32) / 256.0f;
    p4->v = (top*32) / 160.0f;
    p4->r = 1.0; p4->g = 1.0; p4->b = 1.0; p4->a = 1.0;

    p5->u = (left*32+32) / 256.0f;
    p5->v = (top*32+32) / 160.0f;
    p5->r = 1.0; p5->g = 1.0; p5->b = 1.0; p5->a = 1.0;

    p6->u = (left*32) / 256.0f;
    p6->v = (top*32+32) / 160.0f;
    p6->r = 1.0; p6->g = 1.0; p6->b = 1.0; p6->a = 1.0;
}


void chunkCreate(struct Chunk *chunk, struct Tile *tiles, struct Wall *walls, size_t numWalls) {
    struct Array *vertData = arrayCreate(sizeof(struct ChunkVert));
    chunk->numVerts = 0;

    for(int y=0; y < CHUNK_SIZE; y++) {
        for(int x=0; x < CHUNK_SIZE; x++) {
            struct Tile *tile = &tiles[x + y*CHUNK_SIZE];
            struct ChunkVert p1,p2,p3,p4,p5,p6; 

            p1.x = x*TILE_SIZE;
            p1.y = y*TILE_SIZE;
            p1.z = tile->radius;

            p2.x = x*TILE_SIZE+TILE_SIZE;
            p2.y = y*TILE_SIZE;
            p2.z = tile->radius;

            p3.x = x*TILE_SIZE+TILE_SIZE;
            p3.y = y*TILE_SIZE+TILE_SIZE;
            p3.z = tile->radius;

            p4.x = x*TILE_SIZE;
            p4.y = y*TILE_SIZE;
            p4.z = tile->radius;

            p5.x = x*TILE_SIZE+TILE_SIZE;
            p5.y = y*TILE_SIZE+TILE_SIZE;
            p5.z = tile->radius;

            p6.x = x*TILE_SIZE;
            p6.y = y*TILE_SIZE+TILE_SIZE;
            p6.z = tile->radius;

            getTexLoc(tile->type, &p1, &p2, &p3, &p4, &p5, &p6);
            arrayPush(vertData, &p1);
            arrayPush(vertData, &p2);
            arrayPush(vertData, &p3);
            arrayPush(vertData, &p4);
            arrayPush(vertData, &p5);
            arrayPush(vertData, &p6);
            chunk->numVerts += 6;
        }
    }

    for(size_t i=0; i < numWalls; i++) {
        struct Wall *wall = &walls[i];
        struct ChunkVert p1,p2,p3,p4,p5,p6; 

        p1.x = wall->x*TILE_SIZE;
        p1.y = wall->y*TILE_SIZE;
        p1.z = wall->higher;

        p2.x = wall->x*TILE_SIZE+TILE_SIZE;
        p2.y = wall->y*TILE_SIZE;
        p2.z = wall->higher;

        p3.x = wall->x*TILE_SIZE+TILE_SIZE;
        p3.y = wall->y*TILE_SIZE;
        p3.z = wall->lower;

        p4.x = wall->x*TILE_SIZE;
        p4.y = wall->y*TILE_SIZE;
        p4.z = wall->higher;

        p5.x = wall->x*TILE_SIZE+TILE_SIZE;
        p5.y = wall->y*TILE_SIZE;
        p5.z = wall->lower;

        p6.x = wall->x*TILE_SIZE;
        p6.y = wall->y*TILE_SIZE;
        p6.z = wall->lower;

        getTexLoc(wall->type, &p1, &p2, &p3, &p4, &p5, &p6);
        arrayPush(vertData, &p1);
        arrayPush(vertData, &p2);
        arrayPush(vertData, &p3);
        arrayPush(vertData, &p4);
        arrayPush(vertData, &p5);
        arrayPush(vertData, &p6);
        chunk->numVerts += 6;
    }

    glGenVertexArrays(1, &chunk->vao);
    glBindVertexArray(chunk->vao);

    glGenBuffers(1, &chunk->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
    glBufferData(GL_ARRAY_BUFFER, arrayLength(vertData)*sizeof(struct ChunkVert), arrayGetData(vertData), GL_STATIC_DRAW);

    GLint pos = renderGetWrldVertPos();
    GLint tex = renderGetWrldTexCoord();
    GLint col = renderGetWrldVertColour();
    struct ChunkVert *p = NULL;

    glEnableVertexAttribArray(pos);
    glEnableVertexAttribArray(tex);
    glEnableVertexAttribArray(col);

    glVertexAttribPointer(
            pos,
            3,
            GL_FLOAT,
            false,
            sizeof(struct ChunkVert),
            &p->x);

    glVertexAttribPointer(
            tex,
            2,
            GL_FLOAT,
            false,
            sizeof(struct ChunkVert),
            &p->u);

    glVertexAttribPointer(
            col,
            4,
            GL_FLOAT,
            false,
            sizeof(struct ChunkVert),
            &p->r);

    glBindVertexArray(0);

    arrayDestroy(vertData);
}

void chunkDestroy(struct Chunk *chunk) {
    glDeleteVertexArrays(1, &chunk->vao);
    glDeleteBuffers(1, &chunk->vbo);
}

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util/array.h"
#include "map.h"
#include "chunk.h"

struct Header {
    char header[4];
    int32_t width;
    int32_t height;
    int32_t numXChunk;
    int32_t numYChunk;
};

struct Map {
    int width;
    int height;
    int numXChunk;
    int numYChunk;
    struct Array *chunks;
};

static void loadChunk(struct Map *map, FILE *fp) {
}

static void readMapFile(struct Map *map, const char *file) {
    FILE *fp = fopen(file, "r");

    struct Header header;
    fread(&header, sizeof(struct Header), 1, fp);    

    if(header.header[0] != 'M' || header.header[1] != 'A' || 
       header.header[2] != 'P' || header.header[3] != 'F') {

        printf("Incorrect file header in file %s\n", file);
        fclose(fp);
    }

    map->width = header.width;
    map->height = header.height;
    map->numXChunk = header.numXChunk;
    map->numYChunk = header.numYChunk;

    for(int i=0; i < map->numXChunk*map->numYChunk; i++) {
        loadChunk(map, fp);
    }


    fclose(fp);

}

struct Map *mapLoad(const char *file) {
    struct Map *map = malloc(sizeof(struct Map));
    map->chunks = arrayCreate(sizeof(struct Chunk));

    readMapFile(map, file);

    return map;
}

void mapDestroy(struct Map *map) {
    arrayDestroy(map->chunks);
    free(map);
}

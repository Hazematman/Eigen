#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "util/array.h"
#include "map.h"
#include "chunk.h"

struct Tile {
    uint16_t type;
    float radius;
};

struct Wall {
    uint16_t type;
    float lower;
    float higher;
};

struct Header {
    char header[4];
    int32_t width;
    int32_t height;
    int32_t numWalls;
};

struct Map {
    struct Array *chunks;
    struct Wall *walls;
    struct Tile *tiles;
};

static void readMapFile(struct Map *map, const char *file) {
    FILE *fp = fopen(file, "r");

    struct Header header;
    fread(&header, sizeof(struct Header), 1, fp);    

    if(header.header[0] != 'M' || header.header[1] != 'A' || 
       header.header[2] != 'P' || header.header[3] != 'F') {

        printf("Incorrect file header in file %s\n", file);
        fclose(fp);
    }

    map->walls = malloc(sizeof(struct Wall)*header.numWalls);
    fread(map->walls, sizeof(struct Wall), header.numWalls, fp);

    map->tiles = malloc(sizeof(struct Tile)*header.width*header.height);
    fread(map->tiles, sizeof(struct Tile), header.width*header.height, fp);

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
    free(map->tiles);
    free(map->walls);

    free(map);
}

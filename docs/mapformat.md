Map File Format
===============

This document describes the layout of the map format used for the farmer
game. The farmer game uses a little endian binary file format to represent
the map data and it is organized as following.

# Outline
```
#define CHUNK_SIZE 16

struct Tile {
    uint32_t type; // The type id of the tile
    float radius; // The radius of this tile
};

struct Wall {
    uint32_t type;
    float lower; // Lower radius
    float higher; // Upper radius
    int32_t x; // X position of wall
    int32_t y; // Y position of wall
};

struct Chunk {
    int32_t numWalls;
    struct Wall walls[numWalls];
    struct Tile tiles[CHUNK_SIZE*CHUNK_SIZE]
};

struct MapFormat {
    char header[4]; // Always "MAPF" for a valid file
    int32_t width; // Width of the map
    int32_t height; // Height of the map
    int32_t numXChunks;
    int32_t numYChunks;
    struct Chunk chunks[numXChunks*numYChunks]; // Chunk data for map
}
```

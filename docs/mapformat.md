Map File Format
===============

This document describes the layout of the map format used for the farmer
game. The farmer game uses a little endian binary file format to represent
the map data and it is organized as following.

# Outline
```
struct Tile {
    uint16_t type; // The type id of the tile
    float radius; // The radius of this tile
};

struct Wall {
    uint16_t type;
    float lower;
    float higher;
};

struct MapFormat {
    char header[4]; // Always "MAPF" for a valid file
    int32_t width; // Width of the map
    int32_t height; // Height of the map
    int32_t numWalls; // Number of wall objects
    struct Wall walls[numWalls]; // Wall object data
    struct Tile data[width*height]; // The tile data for the map;
}
```

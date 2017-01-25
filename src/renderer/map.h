#ifndef MAP_H
#define MAP_H

struct Map;

struct Map *mapLoad(const char *file);
void mapDestroy(struct Map *map);


#endif

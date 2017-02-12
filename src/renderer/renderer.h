#ifndef RENDERER_H
#define RENDERER_H
#include <stdbool.h>
#include "map.h"

struct Texture;

void renderInit();
void renderCleanUp();
void renderClear(bool depth);
void renderSetDim(int width, int height);

struct Texture *renderLoadTexture(const char *file);
void renderFreeTexture(struct Texture *tex);

void renderSetMap(struct Map *map);
void renderSetTiles(struct Texture *tex);
void renderSetPos(float x, float y);
void renderDrawScene();

int renderGetWrldVertPos();
int renderGetWrldTexCoord();
int renderGetWrldVertColour();

#endif

#ifndef RENDERER_H
#define RENDERER_H
#include <stdbool.h>
#include "map.h"
#include "texture.h"
#include "sprite.h"
#include "billboard.h"

void renderInit();
void renderCleanUp();
void renderClear(bool depth);
void renderSetDim(int width, int height);

struct Texture *renderLoadTexture(const char *file);
void renderFreeTexture(struct Texture *tex);

void renderSetMap(struct Map *map);
void renderSetTiles(struct Texture *tex);
void renderSetPos(float x, float y);
void renderAddSprite(struct Sprite *spr);
void renderRemoveSprite(struct Sprite *spr);
void renderAddBillboard(struct Billboard *bb);
void renderRemoveBillboard(struct Billboard *bb);
void renderDrawScene();

int renderGetWrldVertPos();
int renderGetWrldTexCoord();
int renderGetWrldVertColour();

#endif

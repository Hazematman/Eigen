#ifndef SPRITE_H
#define SPRITE_H
#include "texture.h"

struct Sprite;

struct Sprite *spriteCreate(struct Texture *tex);
void spriteDestroy(struct Sprite *spr);

void spriteSetPos(struct Sprite *spr, float x, float y, float radius);

#endif

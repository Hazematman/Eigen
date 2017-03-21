#ifndef BILLBOARD_H
#define BILLBOARD_H
#include "texture.h"

struct Billboard;

struct Billboard *billboardCreate(struct Texture *tex);
void billboardDestroy(struct Billboard *spr);

void billboardSetPos(struct Billboard *spr, float x, float y, float radius);

#endif

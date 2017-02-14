#include <stdlib.h>
#include "sprite.h"
#include "render_int.h"

struct Sprite *spriteCreate(struct Texture *tex) {
    struct Sprite *spr = malloc(sizeof(struct Sprite));
    spr->tex = tex;

    spr->x = 0;
    spr->y = 0;
    spr->radius = 0;

    return spr;
}

void spriteDestroy(struct Sprite *spr) {
    free(spr);
}

void spriteSetPos(struct Sprite *spr, float x, float y, float radius) {
    spr->x = x;
    spr->y = y;
    spr->radius = radius;
}

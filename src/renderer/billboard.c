#include <stdlib.h>
#include "billboard.h"
#include "render_int.h"

struct Billboard *billboardCreate(struct Texture *tex) {
    struct Billboard *bb = malloc(sizeof(struct Billboard));
    bb->tex = tex;

    bb->x = 0;
    bb->y = 0;
    bb->radius = 0;

    return bb;
}

void billboardDestroy(struct Billboard *bb) {
    free(bb);
}

void billboardSetPos(struct Billboard *bb, float x, float y, float radius) {
    bb->x = x;
    bb->y = y;
    bb->radius = radius;
}

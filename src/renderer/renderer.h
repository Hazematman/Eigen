#ifndef RENDERER_H
#define RENDERER_H
#include <stdbool.h>

void renderInit();
void renderCleanUp();
void renderClear(bool depth);

int renderGetWrldVertPos();
int renderGetWrldTexCoord();
int renderGetWrldVertColour();

#endif

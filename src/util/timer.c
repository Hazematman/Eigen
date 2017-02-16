#include <SDL2/SDL.h>
#include "timer.h"

void timerReset(struct Timer *timer) {
    timer->ticks = SDL_GetTicks();
}

float timerGetElapsedTime(struct Timer *timer) {
    return (SDL_GetTicks() - timer->ticks)/1000.0f;
}

uint32_t timerGetElapsedTicks(struct Timer *timer) {
    return (SDL_GetTicks() - timer->ticks);
}

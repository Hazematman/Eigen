#include <stdlib.h>
#include <SDL2/SDL.h>
#include "error.h"

void logError(const char *errMsg) {
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", errMsg, NULL);
    exit(0);
}

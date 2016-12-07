#include <stdlib.h>
#include <SDL2/SDL.h>
#include "error.h"

void logErrorMsg(const char *file, const char *func, int line, const char *errMsg) {
    printf("Error in %s on line %d from %s:\n%s\n", func, line, file, errMsg);    
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", errMsg, NULL);
    exit(0);
}

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "renderer/renderer.h"
#include "renderer/map.h"
#include "error.h"

#define BITS_PER_PIXEL 8

static SDL_Window *window;
static SDL_GLContext context;

static void checkSDLError() {
    const char *errMsg = SDL_GetError();

    if(*errMsg != '\0') {
        logError(errMsg);
    }
}

static void gameInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        logError("Failed to initalize SDL");
    }
    checkSDLError();

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, BITS_PER_PIXEL);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, BITS_PER_PIXEL);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, BITS_PER_PIXEL);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, BITS_PER_PIXEL);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    window = SDL_CreateWindow("Farmer",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                800, 600,
                                SDL_WINDOW_OPENGL);
    checkSDLError();
    context = SDL_GL_CreateContext(window);
    checkSDLError();

    SDL_GL_SetSwapInterval(1);

    renderInit();
    renderSetDim(800, 600);
}

static void gameRun() {
    SDL_Event e;
    bool running = true;

    struct Map *map = mapLoad("data/maps/test.map");
    struct Texture *tex = renderLoadTexture("data/textures/tiles.jpg");
    renderSetMap(map);
    renderSetTiles(tex);

    float x=0,y=0;
    uint8_t *keys = (uint8_t*)SDL_GetKeyboardState(NULL);
    while(running) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                running = false;
            }
        }

        if(keys[SDL_SCANCODE_LEFT]) {
            x -= 1;
        }
        if(keys[SDL_SCANCODE_RIGHT]) {
            x += 1;
        }
        if(keys[SDL_SCANCODE_UP]) {
            y -= 1;
        }
        if(keys[SDL_SCANCODE_DOWN]) {
            y += 1;
        }

        renderSetPos(x,y);

        renderClear(true);

        renderDrawScene();

        SDL_GL_SwapWindow(window);
    }

    mapDestroy(map);
}

int main() {
    gameInit();

    gameRun();

    renderCleanUp();
    return 0;
}

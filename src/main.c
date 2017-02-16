#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <enet/enet.h>
#include "server/packet.h"
#include "renderer/renderer.h"
#include "renderer/map.h"
#include "server/server.h"
#include "error.h"

#define BITS_PER_PIXEL 8

static SDL_Window *window;
static SDL_GLContext context;
static SDL_Thread *serverThread;

static ENetPeer *serverPeer;
static ENetHost *client = NULL;

static struct Map *map = NULL;
static struct Texture *tex = NULL;

static void checkSDLError() {
    const char *errMsg = SDL_GetError();

    if(*errMsg != '\0') {
        logError(errMsg);
    }
}

static void clientHandle() {
    ENetEvent event;
    PacketType *type;
    while(enet_host_service(client, &event, 0) > 0) {
        switch(event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                type = (PacketType*)event.packet->data;
                if(*type == PACKET_LOAD_MAP) {
                    struct PacketLoadMap *p = (struct PacketLoadMap*)event.packet->data;
                    printf("Attempting to load file %s\n", p->filename);
                    map = mapLoad(p->filename);
                    renderSetMap(map);
                } else if(*type == PACKET_LOAD_PLAYER) {

                }
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                break;
            default:
                break;
        }
    }  
}

static void gameInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        logError("Failed to initalize SDL");
    }
    if(enet_initialize() != 0) {
        logError("Failed to initalize ENet networking library");
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

    tex = renderLoadTexture("./data/textures/tiles.jpg");
    renderSetTiles(tex);
}

static void gameRun() {
    SDL_Event e;
    bool running = true;


    float x=0,y=0;
    uint8_t *keys = (uint8_t*)SDL_GetKeyboardState(NULL);
    while(running) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                running = false;
            }
        }

        if(client != NULL) {
            clientHandle();
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
}

int main(int argc, char *argv[]) {
    bool runServer = false;
    for (int i=1; i < argc; i++) {
        if(strcmp(argv[i],"-server") == 0) {
            runServer = true;
        }
    }

    gameInit();

    if(runServer) {
        serverThread = SDL_CreateThread(serverRun, "ServerThread", (void*)NULL); 
    }

    ENetAddress addr;
    ENetEvent event;

    client = enet_host_create(NULL, 1, 2, 0, 0);
    enet_address_set_host(&addr, "localhost");
    addr.port = 1255;

    serverPeer = enet_host_connect(client, &addr, 2, 0);

    if((enet_host_service(client, &event, 5000) > 0) &&
            event.type == ENET_EVENT_TYPE_CONNECT) {

        printf("[CLIENT] Connected to server\n");
    } else {
        printf("[CLIENT] Failed to connect to server\n");
        return -1;
    }
    
    

    gameRun();

    renderCleanUp();
    return 0;
}

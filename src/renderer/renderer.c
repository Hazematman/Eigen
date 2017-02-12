#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "renderer.h"
#include "gl_core.h"
#include "shaderprg.h"
#include "map_int.h"
#include "math/mat.h"
#include "util/array.h"
#include "../error.h"

struct Texture {
    GLuint id;
    int width, height;
};

struct WorldShd {
    struct ShaderPrg *prg;
    GLint vertPos;
    GLint vertTexCoord;
    GLint vertColour;
    GLint trans;
    GLint texMap;
} worldShd;

static struct Map *sceneMap = NULL;
static struct Texture *tileMap = NULL;
static int renWidth, renHeight;
static float posX=0, posY=0;

static SDL_PixelFormat formatRGB, formatRGBA;

void renderInit() {
    if(ogl_LoadFunctions() == ogl_LOAD_FAILED) {
        logError("Failed to load opengl functions");
    }

    if(ogl_IsVersionGEQ(3,3) == false) {
        logError("OpenGL 3.3 not supported");
    }

    /* Compile world shader program */
    worldShd.prg = shaderPrgCreate("data/shaders/world.vert", "data/shaders/world.frag");
    worldShd.vertPos = shaderGetAttr(worldShd.prg, "vertPos"); 
    worldShd.vertTexCoord = shaderGetAttr(worldShd.prg, "vertTexCoord");
    worldShd.vertColour = shaderGetAttr(worldShd.prg, "vertColour");
    worldShd.trans = shaderGetUniform(worldShd.prg, "trans");
    worldShd.texMap = shaderGetUniform(worldShd.prg, "texMap");

    /* Initalize pixel format types */
    formatRGB.format = SDL_PIXELFORMAT_RGB888;
    formatRGB.palette = NULL;
    formatRGB.BitsPerPixel = 24;
    formatRGB.BytesPerPixel = 3;
    formatRGB.Rmask = 0x0000FF;
    formatRGB.Gmask = 0x00FF00;
    formatRGB.Bmask = 0xFF0000;
    formatRGB.Amask = 0x000000;

    formatRGBA.format = SDL_PIXELFORMAT_RGBA8888;
    formatRGBA.palette = NULL;
    formatRGBA.BitsPerPixel = 32;
    formatRGBA.BytesPerPixel = 4;
    formatRGBA.Rmask = 0x000000FF;
    formatRGBA.Gmask = 0x0000FF00;
    formatRGBA.Bmask = 0x00FF0000;
    formatRGBA.Amask = 0xFF000000;
}

void renderCleanUp() {
    shaderPrgDestroy(worldShd.prg);
}

void renderClear(bool depth) {
    if(depth) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    } else {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void renderSetDim(int width, int height) {
    renWidth = width;
    renHeight = height;
    glViewport(0, 0, width, height);
}

struct Texture *renderLoadTexture(const char *file) {
    SDL_Surface *image = IMG_Load(file);
    if(image == NULL) {
        printf("Could not find image %s\n", file);
        logError("Could not load image");
        return NULL;
    }

    struct Texture *tex = malloc(sizeof(struct Texture));
    tex->width = image->w;
    tex->height = image->h;

    GLenum glFormat = GL_RGB;
    switch(image->format->BytesPerPixel) {
        case 3:
            glFormat = GL_RGB;
            break;
        case 4:
            glFormat = GL_RGBA;
            break;
    }

    SDL_Surface *newImg;
    if(glFormat == GL_RGB) {
        newImg = SDL_ConvertSurface(image, &formatRGB, 0);
    } else {
        newImg = SDL_ConvertSurface(image, &formatRGBA, 0);
    }

    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, glFormat, tex->width, tex->height,
                 0, glFormat, GL_UNSIGNED_BYTE, newImg->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(image);
    SDL_FreeSurface(newImg);

    return tex;
}

void renderFreeTexture(struct Texture *tex) {
    free(tex);
}

int renderGetWrldVertPos() {
    return worldShd.vertPos;
}

int renderGetWrldTexCoord() {
    return worldShd.vertTexCoord;
}

int renderGetWrldVertColour() {
    return worldShd.vertColour;
}

void renderSetMap(struct Map *map) {
    sceneMap = map;
}

void renderSetTiles(struct Texture *tex) {
    tileMap = tex;
}

void renderSetPos(float x, float y) {
    posX = x;
    posY = y;
}

void renderDrawScene() {
    float ortho[16];
    matOrtho(ortho, 0, renWidth, 0, renHeight, -1.0, 1.0);

    glUseProgram(shaderGetId(worldShd.prg));

    if(sceneMap) {
        if(tileMap) {
            glBindTexture(GL_TEXTURE_2D, tileMap->id);
            glUniform1i(worldShd.texMap, 0);
        }


        int iPosX = posX, iPosY = posY;
        int curXChunk = iPosX / (CHUNK_SIZE*TILE_SIZE);
        int curYChunk = iPosY / (CHUNK_SIZE*TILE_SIZE);

        for(int y=curYChunk-1; y <= curYChunk+2; y++) {
            for(int x=curXChunk-1; x <= curXChunk+2; x++) {
                if(x >= 0 && x < sceneMap->numXChunk && y >=0 && y < sceneMap->numYChunk) {
                    float trans[16], out[16];
                    struct Chunk *c = (struct Chunk*)arrayGet(sceneMap->chunks, x + y*sceneMap->numXChunk);
                    matTrans(trans, x*CHUNK_SIZE*TILE_SIZE-posX, y*CHUNK_SIZE*TILE_SIZE-posY, 0); 
                    matMult(out, ortho, trans);

                    glUniformMatrix4fv(worldShd.trans, 1, false, out);

                    glBindVertexArray(c->vao);
                    glDrawArrays(GL_TRIANGLES, 0, c->numVerts);
                }
            }
        }
    }
}

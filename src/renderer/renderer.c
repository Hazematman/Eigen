#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "renderer.h"
#include "gl_core.h"
#include "shaderprg.h"
#include "render_int.h"
#include "math/mat.h"
#include "util/array.h"
#include "../error.h"

struct WorldShd {
    struct ShaderPrg *prg;
    GLint vertPos;
    GLint vertTexCoord;
    GLint vertColour;
    GLint proj;
    GLint trans;
    GLint texMap;
    GLint shift;
} worldShd;

static GLuint quadVbo, quadVao;
static struct ChunkVert quadVerts[] = {
    {-0.5f, 0.0f, 0.1f,
    0.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f},

    {-0.5f, -0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f},

    {0.5f, -0.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f},

    {0.5f, 0.0f, 0.1f,
    1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f},
};

static struct Map *sceneMap = NULL;
static struct Texture *tileMap = NULL;
static int renWidth, renHeight;
static float posX=0, posY=0, shift=1.1;

static struct Array *sprList;

static SDL_PixelFormat formatRGB, formatRGBA;

static void initQuad() {
    glGenVertexArrays(1, &quadVao);
    glBindVertexArray(quadVao);

    glGenBuffers(1, &quadVbo);
    glBindBuffer(GL_ARRAY_BUFFER, quadVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(worldShd.vertPos);
    glEnableVertexAttribArray(worldShd.vertTexCoord);
    glEnableVertexAttribArray(worldShd.vertColour);

    struct ChunkVert *p = NULL;
    glVertexAttribPointer(
            worldShd.vertPos,
            3,
            GL_FLOAT,
            false,
            sizeof(struct ChunkVert),
            &p->x);

    glVertexAttribPointer(
            worldShd.vertTexCoord,
            2,
            GL_FLOAT,
            false,
            sizeof(struct ChunkVert),
            &p->u);

    glVertexAttribPointer(
            worldShd.vertColour,
            4,
            GL_FLOAT,
            false,
            sizeof(struct ChunkVert),
            &p->r);
}

void renderInit() {
    if(ogl_LoadFunctions() == ogl_LOAD_FAILED) {
        logError("Failed to load opengl functions");
    }

    if(ogl_IsVersionGEQ(3,3) == false) {
        logError("OpenGL 3.3 not supported");
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    /* Compile world shader program */
    worldShd.prg = shaderPrgCreate("data/shaders/world.vert", "data/shaders/world.frag");
    worldShd.vertPos = shaderGetAttr(worldShd.prg, "vertPos"); 
    worldShd.vertTexCoord = shaderGetAttr(worldShd.prg, "vertTexCoord");
    worldShd.vertColour = shaderGetAttr(worldShd.prg, "vertColour");
    worldShd.proj = shaderGetUniform(worldShd.prg, "proj");
    worldShd.trans = shaderGetUniform(worldShd.prg, "trans");
    worldShd.texMap = shaderGetUniform(worldShd.prg, "texMap");
    worldShd.shift = shaderGetUniform(worldShd.prg, "shift");

    /* Initalize quad data */
    initQuad();

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

    /* Create array of sprites */
    sprList = arrayCreate(sizeof(struct Sprite*));
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

void renderAddSprite(struct Sprite *spr) {
    for(size_t i=0; i < arrayLength(sprList); i++) {
        struct Sprite *s = *(struct Sprite**)arrayGet(sprList, i);
        if(spr == s) {
            printf("Attempting to add sprite already in scene\n");
            return;
        }
    }

    arrayPush(sprList, &spr);
}

void renderRemoveSprite(struct Sprite *spr) {
    int index = -1;
    for(size_t i=0; i < arrayLength(sprList); i++) {
        struct Sprite *s = *(struct Sprite**)arrayGet(sprList, i);
        if(spr == s) {
            index = i;
        }
    }

    if(index == -1) {
        printf("Could not find sprite to remove!\n");
        return;
    }

    arrayDelete(sprList, index);
}

void renderDrawScene() {
    float ortho[16];
    matOrtho(ortho, 0, renWidth, 0, renHeight, -1.0, 1.0);

    /* Draw the map */
    glUseProgram(shaderGetId(worldShd.prg));
    glUniformMatrix4fv(worldShd.proj, 1, false, ortho);
    glUniform1f(worldShd.shift, shift);
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
                    float trans[16];
                    struct Chunk *c = (struct Chunk*)arrayGet(sceneMap->chunks, x + y*sceneMap->numXChunk);
                    matTrans(trans, x*CHUNK_SIZE*TILE_SIZE-posX, y*CHUNK_SIZE*TILE_SIZE-posY, 0); 

                    glUniformMatrix4fv(worldShd.trans, 1, false, trans);

                    glBindVertexArray(c->vao);
                    glDrawArrays(GL_TRIANGLES, 0, c->numVerts);
                }
            }
        }
    }

    /* Draw all relevant sprites in the scene */
    for(size_t i=0; i < arrayLength(sprList); i++) {
        float trans[16], scale[16], out[16];
        struct Sprite *spr = *(struct Sprite**)arrayGet(sprList, i);
        matScale(scale, spr->tex->width, spr->tex->height, spr->tex->height/32.0f);
        matTrans(trans, spr->x-posX, spr->y-posY, spr->radius);
        matMult(out, trans, scale);

        glUniformMatrix4fv(worldShd.trans, 1, false, out);

        glBindTexture(GL_TEXTURE_2D ,spr->tex->id);
        glUniform1i(worldShd.texMap, 0);

        glBindVertexArray(quadVao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
}

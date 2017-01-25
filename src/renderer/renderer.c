#include "renderer.h"
#include "gl_core.h"
#include "shaderprg.h"
#include "../error.h"

struct WorldShd {
    struct ShaderPrg *prg;
    GLint vertPos;
    GLint vertTexCoord;
    GLint vertColour;
    GLint trans;
    GLint texMap;
} worldShd;

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

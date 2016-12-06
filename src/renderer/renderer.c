#include "renderer.h"
#include "gl_core.h"
#include "../error.h"

void renderInit() {
    if(ogl_LoadFunctions() == ogl_LOAD_FAILED) {
        logError("Failed to load opengl functions");
    }

    if(ogl_IsVersionGEQ(3,3) == false) {
        logError("OpenGL 3.3 not supported");
    }
}

void renderClear(bool depth) {
    if(depth) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    } else {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

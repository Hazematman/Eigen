#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "shaderprg.h"
#include "../error.h"

struct ShaderPrg {
    GLuint prgId;
    GLuint vertex, fragment;
};

static char *loadFile(const char *file) {
    char *data;
    FILE *f = fopen(file, "r");
    if(f == NULL) {
        printf("Failed to open file %s\n", file);
        logError("Failed to shader file");
    }  
    fseek(f, 0, SEEK_END);
    size_t length = ftell(f);
    fseek(f, 0, SEEK_SET);
    data = malloc(sizeof(char)*length);
    if(data == NULL) {
        logError("Out of memory");
    }
    fread(data, 1, length, f);
    fclose (f);

    return data;
}

static void getLog(GLuint obj) {
    GLint logLength = 0;
    if(glIsShader(obj)) {
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &logLength);
    } else {
        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &logLength);
    }

    char *log = malloc(sizeof(char)*logLength);
    if(glIsShader(obj)) {
        glGetShaderInfoLog(obj, logLength, NULL, log);
    } else {
        glGetProgramInfoLog(obj, logLength, NULL, log);
    }

    logError(log);
}

static GLuint compileShader(const char *file, GLenum shader) {
    char *fileData = loadFile(file); 
    GLuint s = glCreateShader(shader);
    glShaderSource(s, 1, (void*)&fileData, NULL);
    glCompileShader(s);

    GLint compiled = false;
    glGetShaderiv(s, GL_COMPILE_STATUS, &compiled);
    if(compiled == false) {
        getLog(s);
    }

    free(fileData);

    return s;
}

struct ShaderPrg *shaderPrgCreate(const char *vFile, const char *fFile) {
    struct ShaderPrg *prg = malloc(sizeof(struct ShaderPrg));
    if(prg == NULL) {
        logError("Out of memory");
    }

    printf("Compiling vertex shader for %s\n", vFile);
    prg->vertex = compileShader(vFile, GL_VERTEX_SHADER);
    printf("Compiling fragment shader for %s\n", fFile);
    prg->fragment = compileShader(fFile, GL_FRAGMENT_SHADER);

    prg->prgId = glCreateProgram();
    glAttachShader(prg->prgId, prg->vertex);
    glAttachShader(prg->prgId, prg->fragment);
    glLinkProgram(prg->prgId);

    GLint linked = false;
    glGetProgramiv(prg->prgId, GL_LINK_STATUS, &linked);
    if(linked == false) {
        getLog(prg->prgId);
    }

    return prg;
}

void shaderPrgDestroy(struct ShaderPrg *prg) {
    glDetachShader(prg->prgId, prg->vertex);
    glDetachShader(prg->prgId, prg->fragment);
    glDeleteShader(prg->vertex);
    glDeleteShader(prg->fragment);
    glDeleteProgram(prg->prgId);
    free(prg);
}

GLuint shaderGetId(struct ShaderPrg *prg) {
    return prg->prgId;
}

GLint shaderGetAttr(struct ShaderPrg *prg, const char *name) {
    GLint loc = glGetAttribLocation(prg->prgId, name);

    if(loc == -1) {
        printf("Error can't find attribute %s\n", name);
        logError("Failed to find attribute");
    }

    return loc;
}

GLint shaderGetUniform(struct ShaderPrg *prg, const char *name) {
    GLint loc = glGetUniformLocation(prg->prgId, name);

    if(loc == -1) {
        printf("Error can't find uniform %s\n", name);
        logError("Failed to find uniform");
    }

    return loc;
}

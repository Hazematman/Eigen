#ifndef SHADERPRG_H
#define SHADERPRG_H
#include "gl_core.h"

struct ShaderPrg;

struct ShaderPrg *shaderPrgCreate(const char *vFile, const char *fFile);
void shaderPrgDestroy(struct ShaderPrg *prg);

GLuint shaderGetId(struct ShaderPrg *prg);
GLint shaderGetAttr(struct ShaderPrg *prg, const char *name);
GLint shaderGetUniform(struct ShaderPrg *prg, const char *name);
#endif

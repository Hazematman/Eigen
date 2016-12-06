#ifndef SHADERPRG_H
#define SHADERPRG_H

struct ShaderPrg;

struct ShaderPrg *shaderPrgCreate(const char *vFile, const char *fFile);
void shaderPrgDestroy(struct ShaderPrg *prg);

#endif

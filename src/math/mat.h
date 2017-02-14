#ifndef MAT_H
#define MAT_H

/* Library for dealing with 4x4 float matricies */

void matIdent(float *mat);
void matOrtho(float *mat, float left, float right , float top, float bottom, float near, float far);
void matTrans(float *mat, float x, float y, float z);
void matScale(float *mat, float sx, float sy, float sz);
void matMult(float *res, float *l, float *r);

void matPrint(float *mat, const char *name);

#endif

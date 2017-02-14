#include <stdio.h>
#include <string.h>
#include "mat.h"

void matIdent(float *mat) {
    mat[0] = 1.0; mat[4] = 0.0; mat[8]  = 0.0; mat[12] = 0.0;
    mat[1] = 0.0; mat[5] = 1.0; mat[9]  = 0.0; mat[13] = 0.0;
    mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
    mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
}

void matOrtho(float *mat, float left, float right, float top, float bottom, float near, float far) {
    matIdent(mat);
    mat[0] = 2.0 / (right - left);
    mat[5] = 2.0 / (top - bottom);
    mat[10] = -2.0 / (far-near);

    mat[12] = -(right+left)/(right-left);
    mat[13] = -(top+bottom)/(top-bottom);
    mat[14] = -(far+near)/(far-near);
    mat[15] = 1.0;
}

void matTrans(float *mat, float x, float y, float z) {
    matIdent(mat);
    mat[12] = x;
    mat[13] = y;
    mat[14] = z;
    mat[15] = 1.0;
}

void matScale(float *mat, float sx, float sy, float sz) {
    matIdent(mat);
    mat[0] = sx;
    mat[5] = sy;
    mat[10] = sz;
}

void matMult(float *res, float *l, float *r) {
    float left[16], right[16];
    memcpy(left, l, sizeof(float)*16);
    memcpy(right, r, sizeof(float)*16);

    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            res[j+i*4] = right[0+i*4]*left[j+0*4] + right[1+i*4]*left[j+1*4] + 
                right[2+i*4]*left[j+2*4] + right[3+i*4]*left[j+3*4];
        }
    }
}


void matPrint(float *mat, const char *name) {
    printf("%s matrix\n", name);
    for(int i=0; i < 4; i++) {
        for(int j=0; j < 4; j++) {
            printf("%f, ", mat[i+j*4]);
        }
        printf("\n");
    }
}

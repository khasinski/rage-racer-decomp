#ifndef GAME_VECTOR_H
#define GAME_VECTOR_H

#include "common.h"

/* Word-sized position/velocity vector. */
typedef struct Vec4 {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Vec4;

/* Four consecutive words moved as a unit by the camera code. */
typedef struct Block16 {
    s32 w0;
    s32 w1;
    s32 w2;
    s32 w3;
} Block16;

#endif

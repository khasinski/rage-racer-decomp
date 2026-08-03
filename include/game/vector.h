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

/* Half-word position/rotation vector; the GTE's SVECTOR shape. */
typedef struct SVec {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} SVec;

/* Half-word screen/point pair; the GTE's DVECTOR shape. */
typedef struct DVec {
    s16 vx;
    s16 vy;
} DVec;

/* Eight-bit colour triplet. */
typedef struct Rgb {
    u8 r;
    u8 g;
    u8 b;
} Rgb;

/* Word position vector without a fourth word. */
typedef struct LVec {
    s32 x;
    s32 y;
    s32 z;
} LVec;

/* Four consecutive words moved as a unit by the camera code. */
typedef struct Block16 {
    s32 w0;
    s32 w1;
    s32 w2;
    s32 w3;
} Block16;

/* Sixteen bytes copied as a unit. */
typedef struct CopyBlock16 {
    u32 word0;
    u32 word1;
    u32 word2;
    u32 word3;
} CopyBlock16;

#endif

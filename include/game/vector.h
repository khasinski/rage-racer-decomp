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

typedef union DVecValue {
    DVec components;
    s32 packed;
} DVecValue;

/* Eight-bit colour triplet. */
typedef struct Rgb {
    u8 r;
    u8 g;
    u8 b;
} Rgb;

/* Colour plus GPU command byte; the SDK's CVECTOR shape. */
typedef struct CVec {
    u8 r;
    u8 g;
    u8 b;
    u8 cd;
} CVec;

/* Word position vector without a fourth word. */
typedef struct LVec {
    s32 x;
    s32 y;
    s32 z;
} LVec;

/* Sixteen bytes moved as a unit; also indexed a word at a time. */
typedef struct Block16 {
    s32 w[4];
} Block16;

#endif

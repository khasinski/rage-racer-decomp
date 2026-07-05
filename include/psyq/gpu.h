#ifndef RAGE_PC_PSYQ_GPU_H
#define RAGE_PC_PSYQ_GPU_H

#include "common.h"

typedef struct Rect {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Rect;

typedef struct GpuRectPacked {
    u32 xy;
    s16 w;
    s16 h;
} GpuRectPacked;

typedef struct DispEnv {
    Rect disp;
} DispEnv;

typedef struct DrawPacket {
    u8 pad0[3];
    u8 code;
    u32 x0y0;
    u32 x1y1;
} DrawPacket;

#endif

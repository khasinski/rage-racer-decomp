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
    Rect screen;
    u8 isinter;
    u8 isrgb24;
} DispEnv;

typedef struct DrawEnv {
    Rect clip;
    s16 ofs[2];
    Rect tw;
    u16 tpage;
    u8 dtd;
    u8 dfe;
} DrawEnv;

typedef struct DrawPacket {
    u8 pad0[3];
    u8 code;
    u32 x0y0;
    u32 x1y1;
} DrawPacket;

typedef struct GpuCallbacks {
    u8 pad0[0x8];
    s32 (*send)(u32 cmd, void *buf, s32 size, u32 data);
    u32 cmd0C;
    void (*submit)(s32 cmd);
    void (*putDispEnv)(void *env, s32 mode);
    u32 moveImage;
    u32 storeImage;
    u32 loadImage;
    u8 pad24[0x28 - 0x24];
    s32 (*read)(s32 cmd);
    void (*clearImage)(void *rect, s32 rgb);
    u8 pad30[0x34 - 0x30];
    void (*resetGraph)(s32 mode);
    s32 (*drawSyncStatus)(void);
    void (*drawSync)(s32 mode);
} GpuCallbacks;

#endif

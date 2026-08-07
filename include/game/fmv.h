#ifndef GAME_FMV_H
#define GAME_FMV_H

#include "common.h"

typedef struct FmvDecodeContext {
    volatile u32 *vlcBuffers[2];
    s32 vlcIndex;
    volatile u32 *stripBuffers[2];
    s32 stripIndex;
} FmvDecodeContext;

extern FmvDecodeContext g_FmvDecodeContext asm("g_FmvVlcBuffers");

#endif

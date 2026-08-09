#ifndef GAME_FMV_H
#define GAME_FMV_H

#include "common.h"

typedef struct FmvDecodeContext {
    volatile u32 *vlcBuffers[2];
    s32 vlcIndex;
    volatile u32 *stripBuffers[2];
    s32 stripIndex;
    struct {
        u16 x;
        u16 y;
        u16 w;
        u16 h;
    } displayRects[2];
    s32 frameParity;
    u16 stripWidth;
    u16 stripHeight;
    u16 sliceHeight;
    u16 decodedHeight;
    s32 decodeComplete;
} FmvDecodeContext;

typedef union FmvStripCursorAddress {
    volatile s32 *index;
    volatile u32 **bufferEnd;
    s32 byteAddress;
} FmvStripCursorAddress;

extern FmvDecodeContext g_FmvDecodeContext asm("g_FmvVlcBuffers");
extern u32 g_FmvRingBuffer;

void StartFmvPlayback();
void InitFmvContext();
void OpenFmvStream();
s32 PresentFmvFrame(FmvDecodeContext *ctx);
void *GetFmvFrame(FmvDecodeContext *ctx);
void StartStreamRead(void *loc);
void WaitFmvDecode(FmvDecodeContext *ctx, s32 mode);

#endif

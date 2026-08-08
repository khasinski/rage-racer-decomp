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
extern u32 g_FmvRingBuffer;

void StartFmvPlayback();
void InitFmvContext();
void OpenFmvStream();
s32 PresentFmvFrame();
void StartStreamRead();
void WaitFmvDecode();
void DecDCTout(volatile u32 *out, s32 words);
void DecDCToutCallback(s32 callback);
s32 StGetBackloc(void *loc);
void StSetRing(s32 base, s32 sectors);
void StSetStream(s32 mode, s32 startFrame, s32 endFrame, s32 doneCallback, s32 errorCallback);
void StCdInterrupt(void);
void StUnSetRing(void);

#endif

#include "common.h"
#include "game/asset.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "game/cd.h"
#include "game/render.h"

extern volatile u32 *g_FmvVlcBuffers[];
extern s32 g_FmvStripIndex;

void DecDCTout(volatile u32 *out, s32 words);
s32 PresentFmvFrame(volatile void *ctx);
s32 StGetBackloc(void *loc);
void StartStreamRead(void *loc);
void WaitFmvDecode(volatile void *ctx, s32 unused);

void DecodeFmvFrame(void) {
    s32 value;
    u8 streamLoc[16];

    g_SceneTimer++;
    if (g_SceneTimer == 4) {
        SetDispMask(1);
    }

    DecDCTin(g_FmvVlcBuffers[g_FmvVlcIndex], 3);
    DecDCTout(g_FmvStripBuffers[g_FmvStripIndex], (g_FmvStripWidth * g_FmvStripHeight) / 2);

    while (PresentFmvFrame(g_FmvVlcBuffers) == -1) {
        value = StGetBackloc(streamLoc);
        DebugPrintf(g_MsgFmvSector, value);
        if ((g_StreamSectorCount < (u32)value) || (value < 0)) {
            StartStreamRead(g_StreamLoc);
        } else {
            StartStreamRead(streamLoc);
        }
    }

    WaitFmvDecode(g_FmvVlcBuffers, 0);
    if (g_FmvStreamEnded == 1) {
        g_FmvState = 2;
    }
    if (g_PadPressed & PAD_START) {
        StartCdVolumeFade(1);
        g_FmvState = 2;
    }
}

extern s32 g_StreamReturnScene;
void DecDCToutCallback(s32 callback);
void StUnSetRing(void);
void EndFmv(void) {
    DecDCToutCallback(0);
    StUnSetRing();
    g_SceneId = g_StreamReturnScene;
    g_StreamReturnScene = g_FmvStreamEnded;
}

extern volatile u32 g_FmvVlcBuffer0;
extern volatile u32 g_FmvVlcBuffer1;
extern volatile u32 g_FmvStripBuffer0;
extern volatile u32 g_FmvStripBuffer1;
extern volatile u16 g_DispEnv0Y;
extern volatile u16 g_DispEnv1X;
extern volatile u16 g_DispEnv1Y;
extern volatile u32 g_FrameParity;

void InitFmvContext(void *ctx, s32 width, s32 height) {
    volatile u32 *words;
    volatile u16 *halves;
    u32 word0;
    u32 word1;
    u32 word3;
    u32 word4;
    u16 half18;
    u16 half1A;
    u16 half20;
    u32 word28;
    u16 half22;

    words = ctx;
    halves = ctx;
    word0 = g_FmvVlcBuffer0;
    word1 = g_FmvVlcBuffer1;
    word3 = g_FmvStripBuffer0;
    word4 = g_FmvStripBuffer1;
    words[2] = 0;
    words[5] = 0;
    words[0] = word0;
    words[1] = word1;
    words[3] = word3;
    words[4] = word4;
    half18 = g_DispEnv0X;
    halves[0xC] = half18;
    half1A = g_DispEnv0Y;
    halves[0xD] = half1A;
    half20 = g_DispEnv1X;
    halves[0x10] = half20;
    word28 = g_FrameParity;
    half22 = g_DispEnv1Y;
    halves[0x16] = width;
    halves[0x17] = height;
    words[0xD] = 0;
    words[0xA] = word28;
    halves[0x18] = 0x18;
    ((u16 *)ctx)[0x11] = half22;
}

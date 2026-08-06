#include "common.h"
#include "game/asset.h"
#include "game/state.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#include "game/cd.h"
#include "game/render.h"

extern volatile u32 *g_FmvVlcBuffers[];
extern s32 g_FmvStripIndex;

void DecDCTout(volatile u32 *arg0, s32 arg1);
s32 PresentFmvFrame(volatile void *arg0);
s32 StGetBackloc(void *arg0);
void StartStreamRead(void *arg0);
void WaitFmvDecode(volatile void *arg0, s32 arg1);

void DecodeFmvFrame(void) {
    s32 value;
    u8 sp10[16];

    g_SceneTimer++;
    if (g_SceneTimer == 4) {
        SetDispMask(1);
    }

    DecDCTin(g_FmvVlcBuffers[g_FmvVlcIndex], 3);
    DecDCTout(g_FmvStripBuffers[g_FmvStripIndex], (g_FmvStripWidth * g_FmvStripHeight) / 2);

    while (PresentFmvFrame(g_FmvVlcBuffers) == -1) {
        value = StGetBackloc(sp10);
        DebugPrintf(g_MsgFmvSector, value);
        if ((g_StreamSectorCount < (u32)value) || (value < 0)) {
            StartStreamRead(g_StreamLoc);
        } else {
            StartStreamRead(sp10);
        }
    }

    WaitFmvDecode(g_FmvVlcBuffers, 0);
    if (g_FmvStreamEnded == 1) {
        g_FmvState = 2;
    }
    if (g_PadEdge2 & 0x800) {
        StartCdVolumeFade(1);
        g_FmvState = 2;
    }
}

extern s32 g_StreamReturnScene;
void DecDCToutCallback(s32 arg0);
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

void InitFmvContext(void *arg0, s32 arg1, s32 arg2) {
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

    words = arg0;
    halves = arg0;
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
    halves[0x16] = arg1;
    halves[0x17] = arg2;
    words[0xD] = 0;
    words[0xA] = word28;
    halves[0x18] = 0x18;
    ((u16 *)arg0)[0x11] = half22;
}

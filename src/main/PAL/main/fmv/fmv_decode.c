#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#include "game/cd.h"

extern volatile u32 *g_FmvVlcBuffers[] asm("D_8009AF20");
extern volatile u32 *g_FmvStripBuffers[] asm("D_8009AF2C");
extern s32 g_FmvVlcIndex asm("D_8009AF28");
extern s32 g_FmvStripIndex asm("D_8009AF34");
extern s32 g_FmvStreamEnded asm("D_8009AF74");
extern s32 g_FmvState asm("D_8009F094");
extern s16 g_FmvStripWidth asm("D_8009AF50");
extern s16 g_FmvStripHeight asm("D_8009AF52");
extern s32 g_StreamSectorCount asm("D_8019CA1C");
extern s32 g_StreamLoc asm("D_801E8A90");
extern char g_MsgFmvSector[] asm("D_80010D30");

void DecDCTin(volatile u32 *arg0, s32 arg1) asm("func_80063FB0");
void DecDCTout(volatile u32 *arg0, s32 arg1) asm("func_8006402C");
s32 PresentFmvFrame(volatile void *arg0) asm("func_8001ED3C");
s32 StGetBackloc(void *arg0) asm("func_8006CF08");
void StartStreamRead(void *arg0) asm("func_8001F018");
void WaitFmvDecode(volatile void *arg0, s32 arg1) asm("func_8001EF54");

void DecodeFmvFrame(void) asm("func_8001E8A4");
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
            StartStreamRead((void *)g_StreamLoc);
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

extern s32 g_StreamReturnScene asm("D_8019C760");
void DecDCToutCallback(s32 arg0) asm("func_800640B0");
void StUnSetRing(void) asm("func_8006CE20");
void EndFmv(void) asm("func_8001EA34");
void EndFmv(void) {
    DecDCToutCallback(0);
    StUnSetRing();
    g_SceneId = g_StreamReturnScene;
    g_StreamReturnScene = g_FmvStreamEnded;
}

extern volatile u32 g_FmvVlcBuffer0 asm("D_8009AF5C");
extern volatile u32 g_FmvVlcBuffer1 asm("D_8009AF60");
extern volatile u32 g_FmvStripBuffer0 asm("D_8009AF64");
extern volatile u32 g_FmvStripBuffer1 asm("D_8009AF68");
extern volatile u16 g_DispEnv0X asm("D_8019CE94");
extern volatile u16 g_DispEnv0Y asm("D_8019CE96");
extern volatile u16 g_DispEnv1X asm("D_801C067C");
extern volatile u16 g_DispEnv1Y asm("D_801C067E");
extern volatile u32 g_FrameParity asm("D_801E4B34");

void InitFmvContext(void *arg0, s32 arg1, s32 arg2) asm("func_8001EA7C");
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

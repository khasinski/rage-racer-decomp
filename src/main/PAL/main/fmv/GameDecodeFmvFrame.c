#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#include "game/cd.h"

extern volatile u32 *D_8009AF20[];
extern volatile u32 *D_8009AF2C[];
extern s32 D_8009AF28;
extern s32 D_8009AF34;
extern s32 D_8009AF74;
extern s32 D_8009F094;
extern s16 D_8009AF50;
extern s16 D_8009AF52;
extern s32 D_8019CA1C;
extern s32 D_801E8A90;
extern char D_80010D30[];

void func_80063FB0(volatile u32 *arg0, s32 arg1);
void DecDCTout(volatile u32 *arg0, s32 arg1) asm("func_8006402C");
s32 GamePresentFmvFrame(volatile void *arg0) asm("func_8001ED3C");
s32 func_8006CF08(void *arg0);
void GameStartStreamRead(void *arg0) asm("func_8001F018");
void GameWaitFmvDecode(volatile void *arg0, s32 arg1) asm("func_8001EF54");

void GameDecodeFmvFrame(void) asm("func_8001E8A4");
void GameDecodeFmvFrame(void) {
    s32 value;
    u8 sp10[16];

    g_SceneTimer++;
    if (g_SceneTimer == 4) {
        SetDispMask(1);
    }

    func_80063FB0(D_8009AF20[D_8009AF28], 3);
    DecDCTout(D_8009AF2C[D_8009AF34], (D_8009AF50 * D_8009AF52) / 2);

    {
        s32 fail;

        fail = -1;
        while (GamePresentFmvFrame(D_8009AF20) == fail) {
            value = func_8006CF08(sp10);
            GameDebugPrintf(D_80010D30, value);
            if ((D_8019CA1C < (u32)value) || (value < 0)) {
                GameStartStreamRead((void *)D_801E8A90);
            } else {
                GameStartStreamRead(sp10);
            }
        }
    }

    GameWaitFmvDecode(D_8009AF20, 0);
    if (D_8009AF74 == 1) {
        D_8009F094 = 2;
    }
    if (g_PadEdge2 & 0x800) {
        GameStartCdVolumeFade(1);
        D_8009F094 = 2;
    }
}

extern s32 g_StreamReturnScene asm("D_8019C760");
void DecDCToutCallback(s32 arg0) asm("func_800640B0");
void StUnSetRing(void) asm("func_8006CE20");
void GameEndFmv(void) asm("func_8001EA34");
void GameEndFmv(void) {
    DecDCToutCallback(0);
    StUnSetRing();
    g_SceneId = g_StreamReturnScene;
    g_StreamReturnScene = D_8009AF74;
}

extern volatile u32 D_8009AF5C;
extern volatile u32 D_8009AF60;
extern volatile u32 D_8009AF64;
extern volatile u32 D_8009AF68;
extern volatile u16 D_8019CE94;
extern volatile u16 D_8019CE96;
extern volatile u16 D_801C067C;
extern volatile u16 D_801C067E;
extern volatile u32 D_801E4B34;

void GameInitFmvContext(void *arg0, s32 arg1, s32 arg2) asm("func_8001EA7C");
void GameInitFmvContext(void *arg0, s32 arg1, s32 arg2) {
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
    word0 = D_8009AF5C;
    word1 = D_8009AF60;
    word3 = D_8009AF64;
    word4 = D_8009AF68;
    words[2] = 0;
    words[5] = 0;
    words[0] = word0;
    words[1] = word1;
    words[3] = word3;
    words[4] = word4;
    half18 = D_8019CE94;
    halves[0xC] = half18;
    half1A = D_8019CE96;
    halves[0xD] = half1A;
    half20 = D_801C067C;
    halves[0x10] = half20;
    word28 = D_801E4B34;
    half22 = D_801C067E;
    halves[0x16] = arg1;
    halves[0x17] = arg2;
    words[0xD] = 0;
    words[0xA] = word28;
    halves[0x18] = 0x18;
    ((u16 *)arg0)[0x11] = half22;
}

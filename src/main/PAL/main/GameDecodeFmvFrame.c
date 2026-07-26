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

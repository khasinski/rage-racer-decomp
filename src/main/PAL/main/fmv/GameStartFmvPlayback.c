#include "common.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "game/render.h"

extern s32 D_8009AF6C;
extern s32 D_8009AF70;
extern s32 D_8009AF74;
extern s32 D_8009F094;
extern s32 D_8009E678;
extern volatile u8 D_8009AF20;
extern u8 D_801C0638;
extern u8 D_8019CE50;
extern u8 D_801C068D;
extern u8 D_8019CEA5;
extern s32 D_801E8A90;

void GameApplyCdVolume(void) asm("func_8004310C");
void GameSetupFmvBuffers(s32 arg0) asm("func_8001EB14");
void GameInitFmvContext(volatile void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) asm("func_8001EA7C");
void GameUploadFmvSlice(void) asm("func_8001EBC8");
void GameOpenFmvStream(void (*arg0)(void)) asm("func_8001EB5C");
s32 GamePresentFmvFrame(volatile void *arg0) asm("func_8001ED3C");
void GameStartStreamRead(s32 arg0) asm("func_8001F018");
s32 VSync(s32 mode) asm("func_8006DD30");

void GameStartFmvPlayback(s32 arg0) asm("func_8001E79C");
void GameStartFmvPlayback(s32 arg0) {
    s32 fail;
    char frame_pad[8];

    SetDispMask(0);
    D_8009AF6C = 0;
    D_8009AF70 = 0;
    GameApplyCdVolume();
    GameSetupDisplay240(0, 0, 0);
    D_801C0638 = 0;
    D_8019CE50 = 0;
    D_801C068D = 1;
    D_8019CEA5 = 1;
    GameSetupFmvBuffers(arg0);
    {
        volatile void *buf = &D_8009AF20;
        GameInitFmvContext(buf, 0, 0x18, 0, 0x108);
    }
    GameOpenFmvStream(GameUploadFmvSlice);
    fail = -1;
    while (1) {
        volatile void *buf = &D_8009AF20;
        if (GamePresentFmvFrame(buf) != fail) {
            break;
        }
        GameStartStreamRead(D_801E8A90);
    }
    D_8009AF74 = 0;
    g_SceneTimer = 0;
    D_8009F094 = 1;
    VSync(0);
    D_8009E678 = 0;
}

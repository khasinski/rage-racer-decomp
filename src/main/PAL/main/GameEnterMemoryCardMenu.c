#include "common.h"
#include "game/state.h"
#include "game/menu.h"
extern s32 D_8009B744, D_80082F50, D_80082F54;
extern s32 D_8009B730, D_8009B9A0, D_8009B9A4;
void func_80065860(s32 arg0);
void func_8001C088(s32 arg0, s32 arg1, s32 arg2);
void func_8005EAD0(void);
void GameEnterMemoryCardMenu(void) asm("func_800613B8");
void GameEnterMemoryCardMenu(void) {
    func_80065860(0);
    func_8001C088(0, 0, 0);
    D_8009B744 = 2;
    g_McMenuState = -1;
    g_SceneTimer = 0;
    D_80082F50 = 0;
    D_80082F54 = 0;
    g_McMenuSubState = 1;
    D_8009B730 = 0;
    func_8005EAD0();
    D_8009B9A0 = -8;
    D_8009B9A4 = 0xFF;
    g_SceneId = 0x1A;
}

#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#include "game/render.h"
extern s32 D_8009B744, D_80082F50, D_80082F54;
extern s32 D_8009B730, D_8009B9A0, D_8009B9A4;
void func_8005EAD0(void);
void GameEnterMemoryCardMenu(void) asm("func_800613B8");
void GameEnterMemoryCardMenu(void) {
    SetDispMask(0);
    GameSetupDisplay480(0, 0, 0);
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

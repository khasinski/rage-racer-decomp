#include "common.h"
#include "game/asset.h"
#include "game/state.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#include "game/render.h"
extern s32 g_ImageBlockBuffer asm("D_801E4B30");
extern s32 g_McMenuRowCursor asm("D_80082F54");
extern s32 g_McMenuRowCount asm("D_8009B744");
extern s32 g_McMenuPage asm("D_80082F50");
extern s32 g_McFromLoadMenu asm("D_8009B730");
extern s32 g_McFadeStep asm("D_8009B9A0");
extern s32 g_McFadeLevel asm("D_8009B9A4");
void GameUploadImageAsset(s32 arg0) asm("func_8001A3C0");
void func_8005EAD0(void);
void GameEnterMemoryCardMenuFromLoad(void) asm("func_80061458");
void GameEnterMemoryCardMenuFromLoad(void) {
    SetDispMask(0);
    GameSetupDisplay480(0, 0, 0);
    if (g_AssetLoadState == 0) {
        GameUploadImageAsset(g_ImageBlockBuffer);
        g_McMenuRowCursor = 2;
        g_McMenuRowCount = 3;
        g_McMenuState = -1;
        g_SceneTimer = 0;
        g_McMenuPage = 0;
        g_McMenuSubState = 1;
        g_McFromLoadMenu = 1;
        func_8005EAD0();
        g_McFadeStep = -8;
        g_McFadeLevel = 0xFF;
        g_SceneId = 0x1A;
    }
}

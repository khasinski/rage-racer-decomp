#include "common.h"
#include "game/audio.h"
#include "game/asset.h"
#include "game/state.h"
#include "game/menu.h"
#include "psyq/gpu.h"
#include "game/render.h"
#include "game/memcard.h"
extern s32 g_ImageBlockBuffer;
extern s32 g_McMenuRowCursor;
extern s32 g_McMenuRowCount;
extern s32 g_McMenuPage;
extern s32 g_McFromLoadMenu;
extern s32 g_McFadeStep;
extern s32 g_McFadeLevel;
void UploadImageAsset(s32 arg0);
void EnterMemoryCardMenuFromLoad(void) {
    SetDispMask(0);
    SetupDisplay480(0, 0, 0);
    if (g_AssetLoadState == 0) {
        UploadImageAsset(g_ImageBlockBuffer);
        g_McMenuRowCursor = 2;
        g_McMenuRowCount = 3;
        g_McMenuState = -1;
        g_SceneTimer = 0;
        g_McMenuPage = 0;
        g_McMenuSubState = 1;
        g_McFromLoadMenu = 1;
        StartMemoryCardEvents();
        g_McFadeStep = -8;
        g_McFadeLevel = 0xFF;
        g_SceneId = 0x1A;
    }
}

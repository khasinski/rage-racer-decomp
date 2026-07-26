#include "common.h"
#include "game/state.h"
#include "game/asset.h"

extern u8 *g_AssetBase asm("D_8019C904");
extern u8 *g_AssetSubBlockPtr asm("D_801E8AB0");
extern u8 *D_801E42D0;
extern u8 *g_AssetLoadCursor asm("D_8019CAFC");
void GameUploadImageAsset(void *arg0) asm("func_8001A3C0");
void func_8001A2E0(void *arg0);
void func_8001A40C(void *arg0);
void func_80019EBC(void);

void GameInstallCourseAssets(void) asm("func_80019730");
void GameInstallCourseAssets(void) {
    u8 *base;
    s32 offset0;
    s32 offset1;

    g_AssetBlockPtr = g_AssetBase + *(s32 *)(g_AssetBase + 0);
    GameUploadImageAsset(g_AssetBlockPtr);

    g_AssetBlockPtr = g_AssetBase + *(s32 *)(g_AssetBase + 4);
    GameUploadImageAsset(g_AssetBlockPtr);

    g_AssetBlockPtr = g_AssetBase + *(s32 *)(g_AssetBase + 8);
    func_8001A2E0(g_AssetBlockPtr);

    base = g_AssetBase;
    offset0 = *(s32 *)(base + 0xC);
    offset1 = *(s32 *)(base + 0x10);
    g_AssetBlockPtr = base + offset0;
    g_AssetSubBlockPtr = base + offset1;
    GameUploadImageAsset(g_AssetBlockPtr);

    func_8001A40C(g_AssetBase);

    D_801E42D0 = g_AssetBase;
    GameUploadImageAsset(g_AssetSubBlockPtr);
    func_80019EBC();
    g_AssetLoadCursor = g_AssetBase + 0x38000;
}

s32 GameRequestTrackDataAssets(void) asm("func_80019844");
s32 GameRequestTrackDataAssets(void) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    if (g_MainState == 0xC) {
        g_MainState = 0;
        return 0;
    }

    g_MainState = 0xC;
    g_AssetLoadState = 1;
    return 1;
}

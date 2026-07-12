#include "common.h"
#include "game/state.h"
#include "game/asset.h"

extern s32 D_8009AEFC;

s32 func_800189E4(s32 arg0) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    g_MainState = 6;
    D_8009AEFC = arg0;
    g_AssetLoadState = 1;
    return 1;
}

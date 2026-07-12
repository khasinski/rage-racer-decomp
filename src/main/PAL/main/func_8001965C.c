#include "common.h"
#include "game/state.h"
#include "game/asset.h"


s32 func_8001965C(void) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    if (g_MainState == 0xB) {
        g_MainState = 0;
        return 0;
    }

    g_MainState = 0xB;
    g_AssetLoadState = 1;
    return 1;
}

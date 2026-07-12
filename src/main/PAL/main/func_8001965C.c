#include "common.h"
#include "game/asset.h"

extern s32 D_8007C704;

s32 func_8001965C(void) {
    if (g_AssetLoadState != 0) {
        return 1;
    }

    if (D_8007C704 == 0xB) {
        D_8007C704 = 0;
        return 0;
    }

    D_8007C704 = 0xB;
    g_AssetLoadState = 1;
    return 1;
}

#include "common.h"
#include "game/state.h"
#include "game/asset.h"


void func_80042C94(void);

s32 func_80019580(void) {
    s32 state;

    if (g_AssetLoadState != 0) {
        return 1;
    }

    state = 10;
    if (g_MainState == state) {
        g_MainState = 0;
        return 0;
    }

    func_80042C94();
    g_MainState = state;
    g_AssetLoadState = 1;
    return 1;
}

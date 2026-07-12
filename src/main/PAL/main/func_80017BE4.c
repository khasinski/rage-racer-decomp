#include "common.h"
#include "game/state.h"
#include "game/asset.h"

extern s16 D_8007C700;

void func_80027634(void);
s32 func_8006A534(s32 arg0, s32 arg1);
s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);

void func_80017BE4(void) {
    if (D_8007C700 == 4) {
        func_80027634();
    }

    D_8007C700 = 0;
    g_AssetLoadState = 0;
    g_MainState = 0;
}

s32 func_80017C2C(void) {
    u8 value;

    if (func_8006A534(1, 0) == 0) {
        return 0;
    }

    value = 7;
    return func_8006A5A4(0xE, &value, 0);
}

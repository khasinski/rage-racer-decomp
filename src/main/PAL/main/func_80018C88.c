#include "common.h"
#include "game/asset.h"

extern s32 D_8007C704;
extern s32 D_8009E6A4;
extern s16 D_8019CABC;
extern s32 D_801E428C;
extern s16 D_801E4DAC;
extern s32 D_801E772C[];

s32 func_800632B0(void);
void func_80017BE4(void);

s32 func_80018C88(void) {
    s32 value;

    if (g_AssetLoadState != 0) {
        func_80017BE4();
    }

    if (D_801E4DAC == 0) {
        value = (func_800632B0() & 0xFFF) % (D_801E772C[D_8019CABC] + 1);
        D_8009E6A4 = value;
        if (((D_801E428C & 3) == 3) && (value < 2)) {
            D_8009E6A4 = ((func_800632B0() & 0xFFF) % (D_801E772C[D_8019CABC] - 1)) + 2;
        }
    }

    if (D_8007C704 == 8) {
        D_8007C704 = 0;
        return 0;
    }

    D_8007C704 = 8;
    g_AssetLoadState = 1;
    return 1;
}

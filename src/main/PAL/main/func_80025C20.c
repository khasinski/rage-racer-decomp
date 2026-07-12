#include "common.h"
#include "game/asset.h"

extern s32 D_801E42A0;

void func_80025AC8(void);

void func_80025C20(void) {
    if (g_AssetLoadState == 0) {
        D_801E42A0 = 4;
    }
    func_80025AC8();
}

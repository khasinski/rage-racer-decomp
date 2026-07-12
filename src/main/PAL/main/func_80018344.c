#include "common.h"
#include "game/asset.h"

extern s32 D_8019C904;
extern s32 D_801E4B30;

s32 func_80017C78(s32 arg0, s32 arg1);

void func_80018344(void) {
    if (g_AssetLoadState == 1) {
        if (func_80017C78(6, D_8019C904) != 0) {
            g_AssetLoadState = 0;
            D_801E4B30 = D_8019C904;
        }
    }
}

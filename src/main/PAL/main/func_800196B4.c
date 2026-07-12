#include "common.h"
#include "game/asset.h"

extern s32 D_801E428C;
extern s32 D_8009E6A4;
extern s32 D_8019C904;
extern s32 D_801E4B30;

s32 func_80017C78(s32 arg0, s32 arg1);

void func_800196B4(void) {
    s32 value;

    if (g_AssetLoadState == 1) {
        s32 left = D_801E428C << 1;
        s32 right = (D_8009E6A4 << 3) + 0x57;

        value = func_80017C78(left + right, D_8019C904);
        if (value != 0) {
            g_AssetLoadState = 0;
            D_801E4B30 = value + D_8019C904;
        }
    }
}

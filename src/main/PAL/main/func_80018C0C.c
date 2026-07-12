#include "common.h"
#include "game/asset.h"

extern s32 D_8019C904;
extern s32 D_801E4B30;

s32 func_80017C78(s32, s32);
void func_80017948(s32, s32);
void func_80017A10(s32);

void func_80018C0C(void) {
    s32 ptr;
    s32 offset;

    if (g_AssetLoadState == 1) {
        if (func_80017C78(9, D_8019C904) != 0) {
            func_80017948(D_8019C904 + 4, 0);
            func_80017A10(0);

            ptr = D_8019C904;
            offset = *(s32 *)ptr;
            g_AssetLoadState = 0;
            D_801E4B30 = ptr + offset;
        }
    }
}

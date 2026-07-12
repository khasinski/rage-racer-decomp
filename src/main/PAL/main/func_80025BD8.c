#include "common.h"
#include "game/asset.h"

extern s32 D_8019C99C;

void func_80019730(void);
void func_80019844(void);
void func_80025AC8(void);

void func_80025BD8(void) {
    if (g_AssetLoadState == 0) {
        func_80019730();
        func_80019844();
        D_8019C99C = 1;
    }

    func_80025AC8();
}

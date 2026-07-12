#include "common.h"
#include "game/asset.h"

extern u32 D_801E4178;

void func_80019730(void);
void func_80019844(void);
void func_80026C0C(void);

void func_80026D30(void) {
    if (g_AssetLoadState == 0) {
        func_80019730();
        func_80019844();
        D_801E4178 = 1;
    }

    func_80026C0C();
}

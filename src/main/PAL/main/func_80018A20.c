#include "common.h"
#include "game/asset.h"


s32 func_800189E4(s32 arg0);
void func_80019C04(void);

void func_80018A20(s32 arg0) {
    func_800189E4(arg0);

    while (g_AssetLoadState != 0) {
        func_80019C04();
    }
}

#include "common.h"
#include "game/asset.h"


s32 func_8001882C(s32 arg0);
void func_80019C04(void);

void func_80018868(s32 arg0) {
    func_8001882C(arg0);

    while (g_AssetLoadState != 0) {
        func_80019C04();
    }
}

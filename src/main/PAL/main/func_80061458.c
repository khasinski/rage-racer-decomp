#include "common.h"
#include "game/asset.h"
extern s32 D_801E4B30, D_80082F54, D_8009B744, D_8009B71C, D_801E40B8;
extern s32 D_80082F50, D_8009B72C, D_8009B730, D_8009B9A0, D_8009B9A4, D_801E42E4;
void func_80065860(s32 arg0);
void func_8001C088(s32 arg0, s32 arg1, s32 arg2);
void func_8001A3C0(s32 arg0);
void func_8005EAD0(void);
void func_80061458(void) {
    func_80065860(0);
    func_8001C088(0, 0, 0);
    if (g_AssetLoadState == 0) {
        func_8001A3C0(D_801E4B30);
        D_80082F54 = 2;
        D_8009B744 = 3;
        D_8009B71C = -1;
        D_801E40B8 = 0;
        D_80082F50 = 0;
        D_8009B72C = 1;
        D_8009B730 = 1;
        func_8005EAD0();
        D_8009B9A0 = -8;
        D_8009B9A4 = 0xFF;
        D_801E42E4 = 0x1A;
    }
}

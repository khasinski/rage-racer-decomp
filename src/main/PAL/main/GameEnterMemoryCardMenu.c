#include "common.h"
extern s32 D_8009B744, D_8009B71C, D_801E40B8, D_80082F50, D_80082F54;
extern s32 D_8009B72C, D_8009B730, D_8009B9A0, D_8009B9A4, D_801E42E4;
void func_80065860(s32 arg0);
void func_8001C088(s32 arg0, s32 arg1, s32 arg2);
void func_8005EAD0(void);
void func_800613B8(void) {
    func_80065860(0);
    func_8001C088(0, 0, 0);
    D_8009B744 = 2;
    D_8009B71C = -1;
    D_801E40B8 = 0;
    D_80082F50 = 0;
    D_80082F54 = 0;
    D_8009B72C = 1;
    D_8009B730 = 0;
    func_8005EAD0();
    D_8009B9A0 = -8;
    D_8009B9A4 = 0xFF;
    D_801E42E4 = 0x1A;
}

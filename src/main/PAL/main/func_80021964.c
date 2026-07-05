#include "common.h"
extern s32 D_801E40B8, D_801E42E4, D_8009E6A4;
extern u16 D_801E436E;
void func_80042CCC(s32 arg0);
void func_80018410(void);
void func_800212F0(s32 arg0);
void func_800218A0(s32 arg0);
void func_80021964(void) {
    s32 v = D_801E40B8 - 1;
    D_801E40B8 = v;
    if ((D_801E436E & 0x860) && (u32)v >= 261) {
        func_80042CCC(0xFA);
        D_801E40B8 = 0xFF;
    }
    if (D_801E40B8 == 0) {
        func_80018410();
        func_800212F0(D_8009E6A4);
        D_801E42E4 = 6;
    }
    func_800218A0(D_801E40B8);
}

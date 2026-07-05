#include "common.h"

extern s32 D_8019C768;
extern s32 D_801E40B8;
extern s32 D_801E42E4;

void func_800218A0(s32 arg0);

void func_80021920(void) {
    D_8019C768 = 0x80;
    D_801E42E4 = 0x10;
    D_801E40B8 = 0x22B;
    func_800218A0(0x22B);
}

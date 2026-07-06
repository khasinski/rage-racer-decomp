#include "common.h"

extern s32 D_8019C768;
extern s32 D_801E42E0;
extern s32 D_801E42A0;
extern s32 D_801E42E4;
extern s32 D_801E4178;
extern s32 D_801E6824;
extern s32 D_801E40B8;
extern s32 D_8009E66C;

void func_80065860(s32 arg0);
void func_8001BE9C(s32 arg0, s32 arg1, s32 arg2);

void func_80026B88(void) {
    func_80065860(0);
    func_8001BE9C(0, 0, 0);

    D_8019C768 = 0x80;
    D_801E42E0 = 0x108;
    D_801E42A0 = -4;
    D_801E42E4 = 0x20;
    D_801E4178 = 0;
    D_801E6824 = 0;
    D_801E40B8 = 0;
    D_8009E66C = 3;
}

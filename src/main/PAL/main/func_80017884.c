#include "common.h"

extern u32 D_8007C484;
extern void *D_801E424C[];

void LoadImage(void *rect, void *data) asm("func_80065B24");

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80017884", func_80017884);

void func_80017B44(void *asset, s32 index) {
    D_801E424C[index] = asset;
}

void func_80017B5C(s32 index) {
    LoadImage(&D_8007C484, D_801E424C[index]);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80017884", func_80017B94);

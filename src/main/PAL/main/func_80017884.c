#include "common.h"

extern u32 D_8007C484;
extern void *D_8009E698;
extern void *D_801E424C[];
extern void *D_801E8A54[];

void LoadImage(void *rect, void *data) asm("func_80065B24");

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80017884", func_80017884);

void func_80017B44(void *asset, s32 index) {
    D_801E424C[index] = asset;
}

void func_80017B5C(s32 index) {
    LoadImage(&D_8007C484, D_801E424C[index]);
}

void func_80017B94(void *asset, s32 index) {
    D_801E8A54[index] = asset;
}

void func_80017BAC(s32 index) {
    D_8009E698 = D_801E8A54[index];
}

asm(".globl func_80017BBC\nfunc_80017BBC = func_80017BAC + 0x10");
asm(".word 0x03E00008\n.word 0x00000000");

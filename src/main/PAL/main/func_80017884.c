#include "common.h"

extern u32 D_8007C484;
extern s32 D_8019CACC;
extern u8 D_8019C86C;
extern u8 D_8009EC94;
extern void *D_8009E698;
extern void *D_801E6828;
extern void *D_801E4BC8;
extern void *D_801E424C[];
extern void *D_801E8A54[];

void LoadImage(void *rect, void *data) asm("func_80065B24");

void func_80017884(s32 arg0) {
    register s32 value asm("$2");
    register s32 ptr asm("$2");
    register s32 tmp asm("$3");

    tmp = D_8019CACC;
    value = 0xA;
    *(s32 *)0x1F80006C = value;
    value = 0x80;
    *(u8 *)0x1F800072 = value;
    *(u8 *)0x1F800071 = value;
    *(u8 *)0x1F800070 = value;
    value = 0x2C;
    *(u8 *)0x1F800073 = value;
    value = 0xFF;
    *(u8 *)0x1F800076 = value;
    *(u8 *)0x1F800075 = value;
    *(u8 *)0x1F800074 = value;
    value = 0x3C;
    *(u8 *)0x1F800077 = value;
    value = 0x140;
    *(u16 *)0x1F80007C = value;
    value = 0xF0;
    *(u16 *)0x1F80007E = value;
    ptr = (s32)&D_8019C86C;
    D_801E6828 = (void *)ptr;
    ptr = (s32)&D_8009EC94;
    *(s32 *)0x1F800064 = arg0;
    *(u16 *)0x1F800078 = 0;
    *(u16 *)0x1F80007A = 0;
    D_801E4BC8 = (void *)ptr;
    *(s32 *)0x1F800068 = tmp;
}

asm(".globl func_80017920\nfunc_80017920 = func_80017884 + 0x9C");

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80017884", func_80017948);

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

#include "common.h"

extern volatile u8 D_8019CE38[];
extern u8 D_801C0620[];
extern u8 D_801C067C[];
extern u16 D_801E4B8C;
extern u16 D_801E4B9C;

void func_80069A58(s32, s32);
void func_80069A78(s32);
void func_80064AA8(void *, s32, s32, s32, s32);
u8 *SetDefDrawEnv(void *, s32, s32, s32, s32) asm("func_80064B78");
void func_8001BE94(s32);

void func_8001C088(s32 arg0, s32 arg1, s32 arg2) {
    register s32 a0_save asm("$18") = arg0;
    register s32 a1_save asm("$19") = arg1;
    register s32 a2_save asm("$20") = arg2;
    register u8 *base asm("$17") = (u8 *)D_8019CE38;
    register s32 height asm("$16");
    register u16 *src0 asm("$8");
    register u16 *src1 asm("$7");
    register s32 i asm("$5");
    register s32 offset asm("$4");
    register s32 one asm("$6");
    register s32 stride asm("$3");
    register u16 value asm("$2");

    __asm__("" : : "r"(a0_save), "r"(a1_save), "r"(a2_save));
    func_80069A58(0xA0, 0xF0);
    func_80069A78(0x140);

    height = 0x1E0;
    func_80064AA8(base, 0, 0, 0x140, height);
    func_80064AA8(D_801C0620, 0, 0, 0x140, height);
    SetDefDrawEnv(base + 0x5C, 0, 0, 0x140, height);
    SetDefDrawEnv(D_801C067C, 0, 0, 0x140, height);

    i = 0;
    one = 1;
    src0 = &D_801E4B8C;
    src1 = &D_801E4B9C;
    offset = 0;
    do {
        stride = 0x20000;
        D_8019CE38[offset + 0x16] = one;
        D_8019CE38[offset + 0x18] = one;
        D_8019CE38[offset + 0x19] = a0_save;
        D_8019CE38[offset + 0x1A] = a1_save;
        D_8019CE38[offset + 0x1B] = a2_save;
        value = *src0;
        stride |= 0x37E8;
        *(volatile u16 *)(D_8019CE38 + offset + 0x64) = value;
        value = *src1;
        i++;
        D_8019CE38[offset + 0x86] = one;
        D_8019CE38[offset + 0x88] = 0;
        D_8019CE38[offset + 0x89] = a0_save;
        D_8019CE38[offset + 0x8A] = a1_save;
        D_8019CE38[offset + 0x8B] = a2_save;
        *(volatile u16 *)(D_8019CE38 + offset + 0x66) = value + 0x1D;
        offset += stride;
    } while (i < 2);

    func_8001BE94(0);
    func_8001BE94(1);

    *(u16 *)0x1F80007E = 0x1E0;
}

asm(".globl func_8001C218\nfunc_8001C218 = func_8001C088 + 0x190");

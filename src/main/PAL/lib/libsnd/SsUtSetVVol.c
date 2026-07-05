#include "common.h"

extern volatile u8 D_8009DF20[];
extern volatile u8 D_8009DF22[];
extern volatile u8 D_8009E0A0[];

s32 SsUtSetVVol(s32 arg0, s16 arg1, s16 arg2) asm("func_80078528");

s32 SsUtSetVVol(s32 arg0, s16 arg1, s16 arg2) {
    register s32 ret asm("$2");
    register s32 yarg asm("$3");
    register s32 x asm("$6");
    register s32 y asm("$3");
    register s32 index asm("$4");
    register s32 offset asm("$5");
    register u8 flags asm("$3");

    __asm__ volatile(
        "\t.set\tnoreorder\n"
        "addiu $sp,$sp,-8 # maspsx-keep\n"
        "andi  $2,%2,0xffff # maspsx-keep\n"
        "sltiu $2,$2,0x18 # maspsx-keep\n"
        "bnez  $2,1f # maspsx-keep\n"
        "addu  %0,%3,$zero # maspsx-keep\n"
        "j     2f # maspsx-keep\n"
        "addiu $2,$zero,-1 # maspsx-keep\n"
        "1: # maspsx-keep\n"
        "sll   $2,%4,16 # maspsx-keep\n"
        "sra   $2,$2,16 # maspsx-keep\n"
        "sll   %1,$2,7 # maspsx-keep\n"
        "addu  %1,%1,$2 # maspsx-keep\n"
        "sll   $2,%0,16 # maspsx-keep\n"
        "sra   $2,$2,16 # maspsx-keep\n"
        "sll   %0,$2,7 # maspsx-keep\n"
        "addu  %0,%0,$2 # maspsx-keep\n"
        "\t.set\treorder\n"
        : "=r"(yarg), "=r"(x)
        : "r"(arg0), "r"(arg2), "r"(arg1)
        : "$2", "memory");

    y = yarg;
    index = (s16)arg0;
    offset = index << 4;
    *(volatile s16 *)(D_8009DF22 + offset) = y;
    flags = D_8009E0A0[index];
    ret = 0;
    *(volatile s16 *)(D_8009DF20 + offset) = x;
    flags |= 3;
    D_8009E0A0[index] = flags;

    __asm__ volatile("2:");
    __asm__ volatile("addiu $sp,$sp,8" ::: "memory");
    return ret;
}

#include "common.h"

/*
 * Attempt for func_8001D748.
 *
 * Same C body as raw_shift_base_attempt, with the first call forced through a
 * small noreorder asm block so the raw store can occupy the jal delay slot.
 */

extern u16 D_8007D30C[];
extern u16 D_8007D330[];
extern volatile u16 D_8007D354[];
extern volatile u16 D_8007D378[];
extern volatile u16 D_8019CB38;
extern volatile u16 D_8019CB3A;
extern volatile u16 D_8019CB3C;

void func_8001D5F0(u32 arg0, u32 arg1);
void func_8001D6F4(u32 arg0, u32 arg1);

void func_8001D748(u32 arg0, u32 arg1) {
    register u32 raw asm("$2");
    register u16 *base asm("$16");
    register u16 s1 asm("$17");
    register u16 s2 asm("$18");
    volatile u16 *idx;
    volatile u16 *color;
    u16 c;
    s32 i;

    raw = arg1;
    arg0 <<= 1;
    base = (u16 *)(raw + 0x7060);
    __asm__("" : "=r"(raw), "=r"(base), "=r"(arg0) : "0"(raw), "1"(base), "2"(arg0));
    s1 = *(u16 *)((u8 *)D_8007D30C + arg0);
    s2 = *(u16 *)((u8 *)D_8007D330 + arg0);

    __asm__ volatile(
        "addu\t$4,%0,$0\n"
        "addu\t$5,%1,$0\n"
        ".set\tnoreorder\n"
        "jal\tfunc_8001D5C8\n"
        "sh\t%0,0x7162(%2)\n"
        ".set\treorder"
        :
        : "r"(s1), "r"(s2), "r"(raw)
        : "$4", "$5", "$31");

    i = 0;
    color = &D_8019CB38;
    idx = D_8007D354;
    for (; i < 9; idx++) {
        i++;
        base[idx[0] + 0] = s1;
        base[idx[0] + 1] = color[0];
        base[idx[0] + 2] = s2;
    }

    func_8001D5F0(s1, s2);

    i = 0;
    color = &D_8019CB38;
    idx = D_8007D378;
    for (; i < 4; idx++) {
        i++;
        base[idx[0] + 0] = s1;
        base[idx[0] + 1] = color[0];
        base[idx[0] + 2] = color[1];
        base[idx[0] + 3] = s2;
    }

    func_8001D6F4(s1, s2);

    ((volatile u16 *)base)[0x2C1] = s1;
    ((volatile u16 *)base)[0x2C2] = D_8019CB38;
    ((volatile u16 *)base)[0x2C3] = D_8019CB3A;
    c = D_8019CB3C;
    ((volatile u16 *)base)[0x2C5] = s2;
    ((volatile u16 *)base)[0x2C4] = c;
}

#include "common.h"

extern u16 D_8007D30C[];
extern u16 D_8007D330[];
extern volatile u16 D_8007D368[];
extern volatile u16 D_8007D378[];
extern volatile u16 D_8019CB38;
extern volatile u16 D_8019CB3A;
extern volatile u16 D_8019CB3C;

void func_8001D5C8(u32 arg0, u32 arg1);
void func_8001D5F0(u32 arg0, u32 arg1);
void func_8001D6F4(u32 arg0, u32 arg1);

void func_8001D900(u32 arg0, u32 arg1) {
    register u16 *base asm("$16");
    register u16 s1 asm("$17");
    register u16 s2 asm("$18");
    volatile u16 *idx;
    volatile u16 *color;
    u16 c;
    s32 i;

    base = (u16 *)(arg1 + 0x7060);
    s1 = D_8007D30C[arg0];
    s2 = D_8007D330[arg0];

    func_8001D5C8(s1, s2);

    i = 0;
    color = &D_8019CB38;
    idx = D_8007D368;
    for (; i < 8; idx++) {
        i++;
        base[idx[0] + 3] = s1;
        base[idx[0] + 4] = color[0];
        base[idx[0] + 5] = s2;
    }

    func_8001D5F0(s1, s2);

    i = 0;
    color = &D_8019CB38;
    idx = D_8007D378;
    for (; i < 4; idx++) {
        i++;
        base[idx[0] + 4] = s1;
        base[idx[0] + 5] = color[0];
        base[idx[0] + 6] = color[1];
        base[idx[0] + 7] = s2;
    }

    func_8001D6F4(s1, s2);

    ((volatile u16 *)base)[0x2C6] = s1;
    ((volatile u16 *)base)[0x2C7] = D_8019CB38;
    ((volatile u16 *)base)[0x2C8] = D_8019CB3A;
    c = D_8019CB3C;
    ((volatile u16 *)base)[0x2CA] = s2;
    ((volatile u16 *)base)[0x2C9] = c;
}

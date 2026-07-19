#include "common.h"

extern s32 D_8007FB2C;
extern s32 D_8019CB0C;

void func_80047BD4(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8);
void func_80046A2C(void *a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7,
                   s32 a8, s32 a9, s32 a10, s32 a11, s32 a12, s32 a13);
void func_80048B88(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7,
                   s32 a8, s32 a9, s32 a10);

void func_8004F3EC(s32 arg0, s32 arg1) {
    register s32 count asm("$23");
    register s32 a1v asm("$7");
    register s32 c17 asm("$17");
    register s32 c19 asm("$19");
    register s32 c21 asm("$21");
    register s32 r4 asm("$4");
    register s32 r5 asm("$5");
    register s32 r6 asm("$6");
    void *ot;
    s32 v0, v1, t, y;

    count = arg0;
    asm("" : "=r"(count) : "0"(count));
    ot = *(void **)0x1F800004;
    asm("" : "=r"(ot) : "0"(ot));
    a1v = arg1;
    asm("" : "=r"(a1v) : "0"(a1v));

    if (count == 0) {
        asm("sw $0,%0" : "=m"(D_8007FB2C));
        return;
    }
    if (count < 0) {
        v0 = count + D_8007FB2C;
        D_8007FB2C = v0;
        if (v0 < 0) {
            D_8007FB2C = 0;
        }
    }
    v0 = D_8007FB2C;
    v1 = v0 - 11;
    if (v1 >= 0 && D_8019CB0C == 0) {
        if (v1 >= 11) {
            v1 = 10;
        }
        r4 = 0x2C;
        t = ((u32)(v1 * -1120)) >> 5;
        y = (s16)(t + 0x21B);
        r5 = y;
        r6 = 7;
        c17 = 0x7F;
        c21 = 0x259;
        c19 = 0x20;
        asm volatile("addiu %0,%1,0x211" : "=r"(t) : "r"(t), "r"(r5), "r"(r6), "r"(c17), "r"(c21), "r"(c19));
        func_80047BD4(r4, r5, r6, a1v, c17, c17, c17, c21, c19);
        func_80047BD4(0x44, y, 7, 0xD, c17, c17, c17, c21, c19);
        func_80046A2C(ot, 0x17, y, 0x34, 0x10, 0x8C, 0x8C, 0, 0, 0, 0x244, 1, 1, 0x3B);
        func_80046A2C(ot, 0x7C, y, 0x8, 0x10, 0x8C, 0xDC, 0, 0, 0, c21, 1, 1, 0x3B);
        func_80048B88(0, (s16)t, 0x99, 0x23, 0, 0, 0, 0, 0, 0, 0);
    }
    if (count > 0) {
        v0 = count + D_8007FB2C;
        D_8007FB2C = v0;
        if (v0 >= 0x1A) {
            D_8007FB2C = 0x19;
        }
    }
}

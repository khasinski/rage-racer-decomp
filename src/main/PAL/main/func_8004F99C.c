#include "common.h"

extern s32 D_8007FB34;
extern s32 D_8019CB0C;

void func_80047BD4(s32 a0, s16 a1, s32 a2, s32 a3, s32 s4, s32 s5, s32 s6, s32 s7, s32 s8);
void func_80046A2C(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 u0, u16 v0, u8 r, u8 g, u8 b, u16 clutX, s32 shadeTex, s32 semiTrans, u32 flags);
void func_80048B88(s32 a0, s16 a1, s32 a2, s32 a3, s32 s4, s32 s5, s32 s6, s32 s7, s32 s8, s32 s9, s32 s10);

void func_8004F99C(s32 arg0, s32 arg1, s32 arg2) {
    void *ot = *(void **)0x1F800004;
    s32 v0;
    s32 v1;
    u32 base;
    s16 s2;
    s16 s3;

    if (arg0 == 0) {
        D_8007FB34 = 0;
    } else {
        if (arg0 < 0) {
            D_8007FB34 += arg0;
            if (D_8007FB34 < 0) {
                D_8007FB34 = 0;
            }
        }
        v0 = D_8007FB34;
        v1 = v0 - 11;
        if (v1 >= 0 && D_8019CB0C == 0) {
            if (v1 >= 11) {
                v1 = 10;
            }
            base = (u32)-(v1 * 1120) >> 5;
            s2 = base + 502;
            func_80047BD4(0x39, s2, 7, arg1, 0x7f, 0x7f, 0x7f, 0x259, 0x20);
            s3 = base + 542;
            func_80047BD4(0x39, s3, 7, arg2, 0x7f, 0x7f, 0x7f, 0x259, 0x20);
            func_80046A2C(ot, 0x17, s2, 0x1d, 0x10, 0x1b, 0x8c, 0, 0, 0, 0x244, 1, 1, 0x3b);
            func_80046A2C(ot, 0x18, s3, 0x34, 0x10, 0x54, 0x8c, 0, 0, 0, 0x244, 1, 1, 0x3b);
            func_80046A2C(ot, 0x89, s2, 0xc, 0x10, 0x50, 0xdc, 0, 0, 0, 0x259, 1, 1, 0x3b);
            func_80046A2C(ot, 0x89, s3, 0xc, 0x10, 0x50, 0xdc, 0, 0, 0, 0x259, 1, 1, 0x3b);
            func_80048B88(0, (s16)(base + 492), 0x99, 0x23, 0, 0, 0, 0, 0, 0, 0);
            func_80048B88(0, (s16)(base + 532), 0x99, 0x23, 0, 0, 0, 0, 0, 0, 0);
        }

        if (arg0 > 0) {
            D_8007FB34 += arg0;
            if (D_8007FB34 >= 26) {
                D_8007FB34 = 25;
            }
        }
    }
}

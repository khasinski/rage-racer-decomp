#include "common.h"

extern s32 D_8007FB30;
extern s32 D_8019CB0C;

void func_80047BD4(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8);
void func_80046A2C(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 u0, u16 v0, u8 r, u8 g,
                   u8 b, u16 clutX, s32 shadeTex, s32 semiTrans, u32 flags);
void func_80048B88(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8,
                   s32 a9, s32 a10);

void func_8004F650(s32 count, s32 arg1, s32 arg2) {
    void *ot;
    s32 v0, v1;
    s16 x2, x3;
    u32 t;

    ot = *(void **)0x1F800004;

    if (count == 0) {
        D_8007FB30 = 0;
        return;
    }
    if (count < 0) {
        v0 = count + D_8007FB30;
        D_8007FB30 = v0;
        if (v0 < 0) {
            D_8007FB30 = 0;
        }
    }
    v0 = D_8007FB30;
    v1 = v0 - 11;
    if (v1 >= 0 && D_8019CB0C == 0) {
        if (v1 >= 11) {
            v1 = 10;
        }
        t = ((u32)(v1 * -1120)) >> 5;
        x2 = t + 502;
        x3 = t + 542;
        func_80047BD4(0x39, x2, 7, arg1, 0x7F, 0x7F, 0x7F, 0x259, 0x20);
        func_80047BD4(0x39, x3, 7, arg2, 0x7F, 0x7F, 0x7F, 0x259, 0x20);
        func_80046A2C(ot, 0x17, x2, 0x1D, 0x10, 0x1B, 0x8C, 0, 0, 0, 0x244, 1, 1, 0x3B);
        func_80046A2C(ot, 0x18, x3, 0x18, 0x10, 0x3C, 0x8C, 0, 0, 0, 0x244, 1, 1, 0x3B);
        func_80046A2C(ot, 0x89, x2, 0xC, 0x10, 0x50, 0xDC, 0, 0, 0, 0x259, 1, 1, 0x3B);
        func_80046A2C(ot, 0x89, x3, 0xC, 0x10, 0x50, 0xDC, 0, 0, 0, 0x259, 1, 1, 0x3B);
        func_80048B88(0, (s16)(t + 492), 0x99, 0x23, 0, 0, 0, 0, 0, 0, 0);
        func_80048B88(0, (s16)(t + 532), 0x99, 0x23, 0, 0, 0, 0, 0, 0, 0);
    }
    if (count > 0) {
        v0 = count + D_8007FB30;
        D_8007FB30 = v0;
        if (v0 >= 26) {
            D_8007FB30 = 0x19;
        }
    }
}

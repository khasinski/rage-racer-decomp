#include "common.h"

extern s32 D_8007FB24;
extern u16 D_801E444E[];

void func_80046A2C(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u8 u0, u8 v0, u8 r, u8 g,
                   u8 b, u16 clutX, s32 shadeTex, s32 semiTrans, u32 flags);
void func_80047024(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha);
void func_80047460(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 arg5, s32 color, s32 arg7,
                   s32 arg8);

void func_8004E368(s32 arg0, s32 arg1) {
    void *ot = *(void **)0x1F800004;
    s32 idx;
    u32 t;
    s16 y;
    s32 i;
    s32 xoff;
    s32 xc = 0xA2;

    if (arg0 == 0) {
        D_8007FB24 = 0;
        return;
    }
    if (arg0 < 0) {
        D_8007FB24 += arg0;
        if (D_8007FB24 < 0) {
            D_8007FB24 = 0;
        }
    }
    idx = D_8007FB24;
    if (idx >= 0) {
        if (idx >= 6) {
            idx = 5;
        }
        t = (u32) - (idx * 960) >> 5;
        y = t + 494;
        func_80046A2C(ot, 0xDA, y, 8, 0x10, (arg1 / 10) << 3, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
        func_80046A2C(ot, 0xE2, y, 8, 0x10, (arg1 % 10) << 3, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
        func_80046A2C(ot, 0xA2, y, 4, 0x10, 0x78, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3A);
        func_80046A2C(ot, 0xA8, y, 0x34, 0x10, 0x7C, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3A);
        func_80046A2C(ot, 0xEA, y, 4, 0x10, 0xB0, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3A);

        for (i = 0, xoff = -23; i < 15; i++) {
            func_80047024(ot, (s16)(xc + xoff), (s16)(t + 528), 8, 0x10,
                          *(u8 *)&D_801E444E[i] << 3, (D_801E444E[i] >> 2) & 0xf8,
                          (D_801E444E[i] >> 7) & 0xf8, 0xFF);
            xoff += 8;
        }

        func_80047460(ot, (s16)(xc - 24), (s16)((u16)y + 32), 0x7A, 0x14, 0xB4, 0xB4, 0xB4, 0xFF);
    }
    if (arg0 > 0) {
        D_8007FB24 += arg0;
        if (D_8007FB24 >= 6) {
            D_8007FB24 = 5;
        }
    }
}

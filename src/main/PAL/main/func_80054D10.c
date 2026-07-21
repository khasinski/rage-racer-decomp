#include "common.h"


INCLUDE_RODATA("asm/PAL/main/nonmatchings/main/func_80054D10", func_80054D10_rodata);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_80054D10", func_80054D10);

extern s32 D_8009B2CC;
extern u8 *D_8009E698;
extern s32 D_801E40D4;
extern u8 *D_8019C7C8;

void func_80047460(void *buf, s32 xa, s32 ya, s32 w, s32 h, s32 r, s32 g, s32 b, s32 code);
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags);

s32 func_800551BC(s32 arg0) {
    s32 p = (s32) *(void **) 0x1F800004;
    u8 *buf = (u8 *) p + 4;
    s32 v;
    s32 col;
    s32 xpos;
    s32 mode;
    u8 tex;

    if (arg0 == 0) {
        D_8009B2CC = 0;
        return p;
    }

    if (arg0 > 0) {
        D_8009B2CC += arg0;
        if (D_8009B2CC >= 509) {
            D_8009B2CC = 508;
        }
    } else {
        D_8009B2CC += arg0;
        if (D_8009B2CC < 0) {
            D_8009B2CC = 0;
        }
    }

    v = (u32) D_8009B2CC >> 2;
    col = v & 0xff;
    func_80047460(buf, 0xa3, 0x180, 0x1a, 0x19, col, col, col, 0x20);

    tex = ((u8 *) D_8019C7C8)[D_801E40D4 * 8 + 2];
    if (tex != 0) {
        func_80046A2C(buf, 0xad, 0x185, 0x10, 0x10, 0x6c, 0x7c, col, col, col,
                      0x244, 0, 1, 0x3b);
        xpos = 0xa5;
    } else {
        func_80046A2C(buf, 0xae, 0x185, 0xc, 0x10, 0x60, 0x7c, col, col, col,
                      0x244, 0, 1, 0x3b);
        xpos = 0xa6;
    }

    mode = D_8009E698[9];
    switch (mode) {
    case 4:
        func_80046A2C(buf, xpos, 0x185, 8, 0x10, 0x20, 0x18, v & 0xff, v & 0xff,
                      v & 0xff, 0x244, 0, 1, 0x3b);
        break;
    case 5:
        func_80046A2C(buf, xpos, 0x185, 8, 0x10, 0x28, 0x18, v & 0xff, v & 0xff,
                      v & 0xff, 0x244, 0, 1, 0x3b);
        break;
    case 6:
        func_80046A2C(buf, xpos, 0x185, 8, 0x10, 0x30, 0x18, v & 0xff, v & 0xff,
                      v & 0xff, 0x244, 0, 1, 0x3b);
        break;
    }

    return D_8009B2CC;
}

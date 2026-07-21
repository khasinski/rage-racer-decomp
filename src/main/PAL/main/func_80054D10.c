#include "common.h"


extern s32 D_8009B2CC;
extern u8 *D_8009E698;
extern s32 D_801E40D4;
extern u8 *D_8019C7C8;

extern s32 D_8009B308;
extern s32 D_8009B2F8;
extern s32 D_8009B2C8;
extern s32 D_8009B318;
extern s32 D_8009B340;
extern s32 D_8009B344;
extern s32 D_8009B334;
extern s32 D_8019CDF8;
extern s32 D_8019CB0C;
extern s32 D_8019C9F0;
extern s32 D_8019C9F8;
extern u16 D_801E436E;
extern s32 D_801E428C;
extern u8 D_80082724;
extern u8 D_80081890;
extern u8 D_80082460;

void func_80047460(void *buf, s32 xa, s32 ya, s32 w, s32 h, s32 r, s32 g, s32 b, s32 code);
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags);

void func_8005194C(void);
void func_8004CF30(s32 arg0);
void func_800489AC(s32 arg0, s32 arg1, s32 arg2);
s32 func_800487D8(u8 *arg0, s32 *arg1, s32 arg2);
void func_8005D6EC(s32 arg0);
s32 func_8004D384(s32 *arg0, s32 arg1, s32 arg2);
void func_800509C4(s32 arg0);

void func_80054D10(void) {
    s32 state;

    D_8019CB0C = 0;
    func_8005194C();
    func_8004CF30(-9);
    state = D_8009B308;
    if (state == 0) {
        D_8009B2F8 = 0;
        D_8009B308 = -1;
        func_800489AC(0, 2, D_8019CDF8);
        func_800487D8(&D_80082724, &D_8009B2F8, 1);
    } else if (state < 0) {
        switch (state) {
        case -1:
            func_800489AC(D_8009B2F8, 2, D_8019CDF8);
            if (func_800487D8(&D_80082724, &D_8009B2F8, 1) != 0) {
                D_8009B318 = -1;
                if (D_801E436E & 0x1000) {
                    func_8005D6EC(1);
                    D_8019CDF8 = (D_8019CDF8 > 0) ? D_8019CDF8 - 1 : 2;
                }
                if (D_801E436E & 0x4000) {
                    func_8005D6EC(1);
                    D_8019CDF8 = (D_8019CDF8 < 2) ? D_8019CDF8 + 1 : 0;
                }
                {
                    s32 flags = D_801E436E;
                    if (flags & 0x860) {
                        s32 x = D_8019CDF8;
                        if (x == 0) {
                            func_8005D6EC(2);
                            D_8009B308 = -2;
                            D_8009B2C8 = -3;
                        } else if (x == 1) {
                            func_8005D6EC(2);
                            D_8009B308 = -2;
                            D_8009B2C8 = -5;
                        } else if (x == 2) {
                            func_8005D6EC(3);
                            D_8009B308 = 1;
                            D_8009B318 = x;
                        }
                    } else if (flags & 0x90) {
                        func_8005D6EC(3);
                        D_8009B308 = 1;
                        D_8009B318 = 2;
                    }
                }
            }
            break;
        case -2:
            func_800487D8(&D_80082724, &D_8009B2F8, -1);
            func_800489AC(D_8009B2F8, 2, D_8019CDF8);
            if (D_8009B2F8 > 0) {
                break;
            }
            D_8009B308 = D_8009B2C8;
            break;
        case -3:
            if (func_8004D384(&D_8009B2F8, 1, 0) == 0) {
                break;
            }
            if (!(D_801E436E & 0x8f0)) {
                break;
            }
            func_8005D6EC(3);
            D_8009B308 = -4;
            break;
        case -4:
            func_8004D384(&D_8009B2F8, -1, 0);
            if (D_8009B2F8 > 0) {
                break;
            }
            D_8009B308 = -1;
            break;
        case -5:
            if (func_8004D384(&D_8009B2F8, 1, 1) == 0) {
                break;
            }
            if (!(D_801E436E & 0x8f0)) {
                break;
            }
            func_8005D6EC(3);
            D_8009B308 = -6;
            break;
        case -6:
            func_8004D384(&D_8009B2F8, -1, 1);
            if (D_8009B2F8 > 0) {
                break;
            }
            D_8009B308 = -1;
            break;
        }
    } else {
        goto pos;
    }
    func_800487D8(&D_80081890, &D_8019C9F0, 0);
    func_800487D8(&D_80082460, &D_8019C9F0, 1);
    return;
pos:
    D_8009B340 = -1;
    D_8009B344 = 2;
    func_800487D8(&D_80082724, &D_8009B2F8, -1);
    func_800489AC(D_8009B2F8, 2, D_8019CDF8);
    func_800487D8(&D_80081890, &D_8019C9F0, -1);
    func_800487D8(&D_80082460, &D_8019C9F0, 0);
    if (D_8019C9F0 > 0) {
        return;
    }
    D_8019C9F8 = 1;
    D_8009B340 = 1;
    D_8019CDF8 = 0;
    D_8019C9F0 = 0;
    D_8009B308 = 0;
    func_800509C4(0);
    if (D_801E428C >= 4) {
        D_8009B334 = 1;
    } else {
        D_8009B334 = -1;
    }
}

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

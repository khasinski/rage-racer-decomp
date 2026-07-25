#include "common.h"
#include "game/car.h"

extern u8 D_80011BA0;
extern u8 D_80082130;
extern u8 D_80082460;
extern u8 D_80082790;
extern u8 D_80082898;
extern u8 D_80082A54;
extern s32 D_80082D80[];
extern u8 D_8009B2F0;
extern s32 D_8009B2F8;
extern s32 D_8009B300;
extern s32 D_8009B308;
extern s32 D_8009B318;
extern s32 D_8009B31C;
extern s32 D_8009B320;
extern s32 D_8009B338;
extern s32 D_8009B340;
extern s32 D_8009B344;
extern s32 D_8009B34C;
extern s32 D_8009B350;
extern s32 D_8009B374;
extern s32 D_8009B378;
extern GameCarEntry *D_8019C7C8;
extern s32 D_8019C908;
extern s32 D_8019C9F0;
extern s32 D_8019C9F8;
extern s32 D_8019CB0C;
extern u8 *D_801E4188;
extern s32 D_801E40D4;
extern s32 D_801E4290;
extern u16 D_801E436E;
extern GameCarEntry D_801E4388[];

void func_800189E4(s32 arg0);
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags);
s32 func_800487D8(u8 *commands, s32 *progress, s32 step);
void func_800489AC(s32 arg0, s32 arg1, s32 arg2);
void func_80048B88(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                   s32 a7, s32 a8, s32 a9, void *a10);
void func_80048D64(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash);
void func_8004F99C(s32 arg0, s32 arg1, s32 arg2);
void func_8004FCE8(s32 arg0, s32 arg1, s32 arg2);
s32 func_80050FA8(s32 arg0);
void func_8005131C(void);
void func_8005D6EC(s32 arg0);

void func_8005A3A4(void) {
    void *ot;
    s32 value;
    s32 res;
    s32 sel;

    ot = *(void **)0x1F800004;
    D_8019CB0C = D_8009B338;
    func_8004FCE8(D_8009B31C, D_8009B320, 0);
    func_8005131C();
    D_8009B320 = D_801E40D4;
    value = D_80082D80[func_80050FA8(D_801E40D4)];
    if (D_8009B308 == 0) {
        func_800487D8(D_801E4188, &D_8009B2F8, -1);
        func_800487D8(&D_80082790, &D_8009B2F8, 0);
        func_8004F99C(1, D_8019C908, value);
        func_800489AC(D_8019C9F0, 1, D_801E4290);
        func_800487D8(&D_80082130, &D_8019C9F0, 0);
        res = func_800487D8(&D_80082460, &D_8019C9F0, 1);
        if ((res != 0) && (D_8009B2F8 <= 0)) {
            D_8009B318 = -1;
            if (D_801E436E & 0x1000) {
                func_8005D6EC(1);
                D_801E4290 = (D_801E4290 > 0) ? D_801E4290 - 1 : 1;
            }
            if (D_801E436E & 0x4000) {
                func_8005D6EC(1);
                D_801E4290 = (D_801E4290 <= 0) ? D_801E4290 + 1 : 0;
            }
            if (D_801E436E & 0x860) {
                sel = D_801E4290;
                if (sel == 0) {
                    if (D_8019C908 >= value) {
                        func_8005D6EC(2);
                        D_801E4188 = &D_80082A54;
                        D_8009B308 = -1;
                        D_8009B2F8 = 0;
                        D_8009B2F0 = 0;
                    } else {
                        func_8005D6EC(5);
                        D_801E4188 = &D_80082898;
                        D_8009B308 = -3;
                        D_8009B2F8 = 0;
                    }
                } else if (sel == 1) {
                    func_8005D6EC(3);
                    D_8009B308 = sel;
                    D_8009B318 = 2;
                }
            } else if (D_801E436E & 0x90) {
                func_8005D6EC(3);
                D_8009B308 = 1;
                D_8009B318 = 2;
            }
        }
    } else {
        if (D_8009B308 < 0) {
            if (D_8009B308 == -1) {
                u16 *pad;

                func_800487D8(D_801E4188, &D_8009B2F8, 0);
                if (func_800487D8(&D_80082790, &D_8009B2F8, 1) != 0) {
                    if (D_801E436E & 0x860) {
                        if (D_8009B2F0 != 0) {
                            func_8005D6EC(2);
                            D_8009B308 = -2;
                            D_8009B300 = 0x23;
                            func_800189E4(D_801E40D4);
                        } else {
                            func_8005D6EC(3);
                            D_8009B308 = 0;
                        }
                    }
                    pad = &D_801E436E;
                    if (*pad & 0x90) {
                        func_8005D6EC(3);
                        D_8009B308 = 0;
                    }
                    if ((*pad & 0x8000) && (D_8009B2F0 == 0)) {
                        func_8005D6EC(1);
                        D_8009B2F0 = 1;
                    }
                    if (D_801E436E & 0x2000) {
                        if (D_8009B2F0 != 0) {
                            func_8005D6EC(1);
                            D_8009B2F0 = 0;
                        }
                    }
                    func_80048D64((D_8009B2F0 != 0) ? 0xB8 : 0xDA, 0x44, 0x20, 0x20, 0);
                    func_80046A2C(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80046A2C(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80048B88(0xB8, 0x44, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &D_80011BA0);
                    func_80048B88(0xDA, 0x44, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &D_80011BA0);
                }
            } else if (D_8009B308 == -2) {
                if (D_8009B300 <= 0) {
                    func_800487D8(D_801E4188, &D_8009B2F8, -1);
                    func_800487D8(&D_80082790, &D_8009B2F8, 0);
                    if (D_8009B2F8 <= 0) {
                        D_8009B34C = 0x927C0;
                        D_8009B350 = 0;
                        D_8009B308 = 2;
                        D_8009B318 = 2;
                        D_8009B374 = D_801E40D4;
                        D_8009B378 = D_801E40D4;
                    }
                } else {
                    D_8009B300 -= 1;
                    func_800487D8(D_801E4188, &D_8009B2F8, 0);
                    func_800487D8(&D_80082790, &D_8009B2F8, 1);
                    func_80048D64((D_8009B2F0 != 0) ? 0xB8 : 0xDA, 0x44, 0x20, 0x20, 1);
                    func_80046A2C(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80046A2C(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80048B88(0xB8, 0x44, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &D_80011BA0);
                    func_80048B88(0xDA, 0x44, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &D_80011BA0);
                }
            } else {
                func_800487D8(D_801E4188, &D_8009B2F8, 0);
                if (func_800487D8(&D_80082790, &D_8009B2F8, 1) != 0) {
                    if (D_801E436E & 0x860) {
                        D_8009B308 = 0;
                    }
                    if (D_801E436E & 0x90) {
                        D_8009B308 = 0;
                    }
                }
            }
            func_8004F99C(1, D_8019C908, value);
            func_800489AC(D_8019C9F0, 1, D_801E4290);
            func_800487D8(&D_80082130, &D_8019C9F0, 0);
            func_800487D8(&D_80082460, &D_8019C9F0, 1);
            return;
        }
        D_8009B340 = -1;
        D_8009B344 = 0xC;
        func_8004F99C(-1, D_8019C908, value);
        func_800487D8(&D_80082130, &D_8019C9F0, -1);
        func_800487D8(&D_80082460, &D_8019C9F0, 0);
        func_800489AC(D_8019C9F0, 1, D_801E4290);
        if (D_8019C9F0 <= 0) {
            if (D_8009B308 == 2) {
                D_8019C7C8[D_801E40D4].modelVariant++;
                if (D_8019C7C8[D_801E40D4].modelVariant > D_801E4388[D_801E40D4].modelVariant) {
                    D_801E4388[D_801E40D4].modelVariant = D_8019C7C8[D_801E40D4].modelVariant;
                }
                D_8019C908 -= value;
            }
            D_8019C9F8 = 4;
            D_8009B340 = 4;
            D_8019C9F0 = 0;
            D_8009B308 = 0;
            D_801E4290 = 0;
        }
    }
}

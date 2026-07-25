#include "common.h"
#include "game/car.h"

extern u8 D_80011BA0;
extern u8 D_80081A34;
extern u8 D_80081AD0;
extern u8 D_80082460;
extern u8 D_80082574;
extern u8 D_800825A4;
extern u8 D_80082790;
extern u8 D_80082814;
extern u8 D_8009B2F0;
extern s32 D_8009B2F8;
extern s32 D_8009B300;
extern s32 D_8009B308;
extern s32 D_8009B318;
extern s32 D_8009B31C;
extern s32 D_8009B320;
extern s32 D_8009B324;
extern s32 D_8009B338;
extern s32 D_8009B340;
extern s32 D_8009B344;
extern s32 D_8009B358;
extern s32 D_8009B35C;
extern u8 *D_8009E698;
extern u8 *D_8019C794;
extern s32 D_8019C7C0;
extern GameCarEntry *D_8019C7C8;
extern s32 D_8019C9F0;
extern s32 D_8019C9F8;
extern s32 D_8019CB0C;
extern s32 D_801E40D4;
extern u16 D_801E436E;
extern u8 D_801E4389[];
extern u8 D_801E438A[];
extern s16 D_801E4DAC;

void func_8004FCE8(s32 arg0, s32 arg1, s32 arg2);
void func_8005131C(void);
s32 func_800487D8(u8 *commands, s32 *progress, s32 step);
void func_800489AC(s32 arg0, s32 arg1, s32 arg2);
void func_8005D6EC(s32 arg0);
void func_80048ED8(u8 x, s32 useFlag);
void func_80048D64(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash);
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags);
void func_80048B88(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                   s32 a7, s32 a8, s32 a9, void *a10);

void func_800563A0(void) {
    void *ot;
    s32 mode;
    s32 lowMode;
    u8 *cmdList;
    u16 *pad;
    s32 sel;

    ot = *(void **)0x1F800004;
    D_8019CB0C = D_8009B338;
    func_8004FCE8(D_8009B31C, D_8009B320, 0);
    mode = 2;
    func_8005131C();
    if (D_801E4DAC != 0) {
        mode = 3;
    }
    cmdList = &D_80081AD0;
    if (D_801E4DAC != 0) {
        cmdList = &D_80081A34;
    }

    if (D_8009B308 == 0) {
        D_8009B324 = 3;
        func_800487D8(D_8019C794, &D_8009B2F8, -1);
        lowMode = mode & 0xFF;
        func_800489AC(D_8019C9F0, lowMode, D_8019C7C0);
        func_800487D8(cmdList, &D_8019C9F0, 0);
        if ((func_800487D8(&D_80082460, &D_8019C9F0, 1) != 0) && (D_8009B2F8 <= 0)) {
            D_8009B318 = -1;
            if (D_801E436E & 0x1000) {
                func_8005D6EC(1);
                D_8019C7C0 = (D_8019C7C0 > 0) ? D_8019C7C0 - 1 : lowMode;
            }
            if (D_801E436E & 0x4000) {
                func_8005D6EC(1);
                D_8019C7C0 = (D_8019C7C0 < mode) ? D_8019C7C0 + 1 : 0;
            }
            if (D_801E436E & 0x860) {
                u8 carByte;

                sel = D_8019C7C0;
                if (sel == 0) {
                    func_8005D6EC(2);
                    carByte = D_8019C7C8[D_801E40D4].pad1[0];
                    D_8019C794 = &D_80082574;
                    D_8009B308 = -1;
                    goto set_state;
                }
                if (sel == 1) {
                    if (D_8009E698[8] != 0) {
                        func_8005D6EC(2);
                        carByte = D_8019C7C8[D_801E40D4].pad1[1];
                        D_8019C794 = &D_800825A4;
                        D_8009B308 = -2;
set_state:
                        D_8009B2F8 = 0;
                        D_8009B2F0 = carByte;
                        return;
                    }
                    func_8005D6EC(5);
                    D_8019C794 = &D_80082814;
                    D_8009B308 = -3;
                    D_8009B2F8 = 0;
                    return;
                }
                if (sel == mode) {
                    goto block27;
                }
                if (sel == 2) {
                    func_8005D6EC(2);
                    D_8009B308 = 1;
                    D_8009B318 = 1;
                    D_8009B324 = -3;
                    D_8009B35C = 0x3D090;
                }
            } else if (D_801E436E & 0x90) {
block27:
                func_8005D6EC(3);
                D_8009B308 = 2;
                D_8009B318 = 2;
            }
        }
        return;
    }

    if (D_8009B308 < 0) {
        if (D_8009B308 == -1) {
            if (func_800487D8(D_8019C794, &D_8009B2F8, 1) != 0) {
                pad = &D_801E436E;
                if (*pad & 0x860) {
                    func_8005D6EC(2);
                    D_8009B308 = -5;
                    D_8009B300 = 0x23;
                }
                if (*pad & 0x90) {
                    func_8005D6EC(3);
                    D_8009B308 = 0;
                }
                if ((*pad & 0x8000) && (D_8009B2F0 < 4)) {
                    func_8005D6EC(1);
                    D_8009B2F0++;
                }
                if (D_801E436E & 0x2000) {
                    if (D_8009B2F0 != 0) {
                        func_8005D6EC(1);
                        D_8009B2F0--;
                    }
                }
                func_80048ED8(D_8009B2F0, 0);
            }
        } else if (D_8009B308 == -2) {
            if (func_800487D8(D_8019C794, &D_8009B2F8, 1) != 0) {
                pad = &D_801E436E;
                if (*pad & 0x860) {
                    func_8005D6EC(2);
                    D_8009B308 = -6;
                    D_8009B300 = 0x23;
                    D_8019C7C8[D_801E40D4].pad1[1] = D_8009B2F0;
                    D_801E438A[D_801E40D4 * 8] = D_8009B2F0;
                }
                if (*pad & 0x90) {
                    func_8005D6EC(3);
                    D_8009B308 = 0;
                }
                if ((*pad & 0x8000) && (D_8009B2F0 != 0)) {
                    func_8005D6EC(1);
                    D_8009B2F0 = 0;
                }
                if (D_801E436E & 0x2000) {
                    if (D_8009B2F0 == 0) {
                        func_8005D6EC(1);
                        D_8009B2F0 = 1;
                    }
                }
                func_80048D64((D_8009B2F0 != 0) ? 0xDA : 0xB8, 0x68, 0x20, 0x20, 0);
                func_80046A2C(ot, 0xC2, 0x70, 0xC, 0x10, 0x60, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80046A2C(ot, 0xE3, 0x70, 0x10, 0x10, 0x6C, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80048B88(0xB8, 0x68, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &D_80011BA0);
                func_80048B88(0xDA, 0x68, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &D_80011BA0);
            }
        } else if (D_8009B308 == -3) {
            func_800487D8(D_8019C794, &D_8009B2F8, 0);
            if (func_800487D8(&D_80082790, &D_8009B2F8, 1) != 0) {
                if (D_801E436E & 0x860) {
                    D_8009B308 = -4;
                }
                if (D_801E436E & 0x90) {
                    D_8009B308 = -4;
                }
            }
        } else if (D_8009B308 == -4) {
            func_800487D8(D_8019C794, &D_8009B2F8, -1);
            func_800487D8(&D_80082790, &D_8009B2F8, 0);
            if (D_8009B2F8 <= 0) {
                D_8009B308 = 0;
            }
        } else if (D_8009B308 == -5) {
            if (D_8009B300 <= 0) {
                func_800487D8(D_8019C794, &D_8009B2F8, -1);
                if (D_8009B2F8 <= 0) {
                    D_8009B308 = 0;
                    D_8019C7C8[D_801E40D4].pad1[0] = D_8009B2F0;
                    D_801E4389[D_801E40D4 * 8] = D_8009B2F0;
                }
            } else {
                D_8009B300 -= 1;
                func_800487D8(D_8019C794, &D_8009B2F8, 1);
                func_80048ED8(D_8009B2F0, 1);
            }
        } else if (D_8009B308 == -6) {
            if (D_8009B300 <= 0) {
                func_800487D8(D_8019C794, &D_8009B2F8, -1);
                if (D_8009B2F8 <= 0) {
                    D_8009B308 = 0;
                }
            } else {
                D_8009B300 -= 1;
                func_800487D8(D_8019C794, &D_8009B2F8, 1);
                func_80048D64((D_8009B2F0 != 0) ? 0xDA : 0xB8, 0x68, 0x20, 0x20, 1);
                func_80046A2C(ot, 0xC2, 0x70, 0xC, 0x10, 0x60, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80046A2C(ot, 0xE3, 0x70, 0x10, 0x10, 0x6C, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80048B88(0xB8, 0x68, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, &D_80011BA0);
                func_80048B88(0xDA, 0x68, 0x20, 0x20, 0x1E, 0x8E, 0x95, 0, 0, 0, &D_80011BA0);
            }
        }
        func_800489AC(D_8019C9F0, mode, D_8019C7C0);
        func_800487D8(cmdList, &D_8019C9F0, 0);
        func_800487D8(&D_80082460, &D_8019C9F0, 1);
        return;
    }

    D_8009B340 = -1;
    D_8009B344 = 5;
    func_800487D8(cmdList, &D_8019C9F0, -1);
    func_800487D8(&D_80082460, &D_8019C9F0, 0);
    func_800489AC(D_8019C9F0, mode, D_8019C7C0);
    if (D_8019C9F0 <= 0) {
        switch (D_8009B308) {
        case 1:
            if (D_8009B358 <= 0x3D08F) {
                return;
            }
            D_8019C9F8 = 6;
            D_8009B340 = 6;
            break;
        case 2:
            D_8019C9F8 = 4;
            D_8009B340 = 4;
            D_8019C7C0 = 0;
            break;
        }
        D_8019C9F0 = 0;
        D_8009B308 = 0;
    }
}

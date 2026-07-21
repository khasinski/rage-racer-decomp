#include "common.h"

extern s32 D_80082EA4;
extern s32 D_80082EA8;
extern s32 D_80082EB0;
extern s32 D_8019CB0C;
extern s32 D_8009B308;
extern s32 D_8009B2F8;
extern s32 D_8009B318;
extern s32 D_8009B340;
extern s32 D_8009B344;
extern u16 D_801E436E;
extern s32 D_8019C770;
extern s32 D_8019C9F0;
extern s32 D_8019C9F8;
extern void *D_8009F0B0;
extern u8 D_80082634;
extern u8 D_80082664;
extern u8 D_80081CA4;
extern u8 D_80082460;

extern void func_8001D338(s32, s32);
extern void func_8004A248(s32, s32);
extern void func_8004B8B4(s32, s32);
extern void func_8004E368(s32, s32);
extern s32 func_800487D8(void *, void *, s32);
extern void func_800489AC(s32, s32, s32);
extern void func_8005D6EC(s32);

void func_800580C8(void) {
    s32 v0;
    s32 t;
    s32 pl;

    D_8019CB0C = 0;
    func_8001D338(D_80082EA4, D_80082EA8);
    func_8004A248(1, 0);
    v0 = D_8009B308;
    if (v0 == 0) {
        func_8004B8B4(-10, 0);
        func_8004E368(-1, D_80082EB0 + 1);
        func_800487D8(D_8009F0B0, &D_8009B2F8, -1);
        func_800489AC(D_8019C9F0, 2, D_8019C770);
        func_800487D8(&D_80081CA4, &D_8019C9F0, 0);
        if (func_800487D8(&D_80082460, &D_8019C9F0, 1) == 0) return;
        if (D_8009B2F8 > 0) return;
        D_8009B318 = -1;
        if (D_801E436E & 0x1000) {
            s32 n, c;
            func_8005D6EC(1);
            c = D_8019C770;
            n = 2;
            if (c > 0) n = c - 1;
            D_8019C770 = n;
        }
        if (D_801E436E & 0x4000) {
            s32 n, c;
            func_8005D6EC(1);
            c = D_8019C770;
            n = 0;
            if (c < 2) n = c + 1;
            D_8019C770 = n;
        }
        if (D_801E436E & 0x860) {
            pl = D_8019C770;
            if (pl == 0) {
                func_8005D6EC(2);
                D_8009B308 = -1;
                D_8009B2F8 = 0;
                D_8009F0B0 = &D_80082634;
                D_80082EB0 = D_80082EA4;
            } else if (pl == 1) {
                func_8005D6EC(2);
                D_8009B308 = -2;
                D_8009B2F8 = 0;
                D_8009F0B0 = &D_80082664;
                D_80082EB0 = D_80082EA8;
            } else if (pl == 2) {
                func_8005D6EC(3);
                D_8009B308 = 1;
                D_8009B318 = 2;
            }
        } else if (D_801E436E & 0x90) {
            func_8005D6EC(3);
            D_8009B308 = 1;
            D_8009B318 = 2;
        }
        return;
    }

    if (v0 < 0) {
        func_8004B8B4(10, 0);
        if (D_8009B308 == -1) {
            if (func_800487D8(D_8009F0B0, &D_8009B2F8, 1) != 0) {
                u16 *p = &D_801E436E;
                if (*p & 0x860) {
                    func_8005D6EC(2);
                    D_8009B308 = 0;
                    D_80082EB0 = D_80082EA4;
                }
                if (*p & 0x90) {
                    func_8005D6EC(3);
                    D_8009B308 = 0;
                    D_80082EA4 = D_80082EB0;
                }
                if (*p & 0x8000) {
                    s32 n, c;
                    func_8005D6EC(1);
                    c = D_80082EA4;
                    n = 0x13;
                    if (c > 0) n = c - 1;
                    D_80082EA4 = n;
                }
                if (D_801E436E & 0x2000) {
                    s32 n, c;
                    func_8005D6EC(1);
                    c = D_80082EA4;
                    n = 0;
                    if (c < 19) n = c + 1;
                    D_80082EA4 = n;
                }
            }
            t = D_80082EA4;
        } else {
            if (func_800487D8(D_8009F0B0, &D_8009B2F8, 1) != 0) {
                u16 *p = &D_801E436E;
                if (*p & 0x860) {
                    func_8005D6EC(2);
                    D_8009B308 = 0;
                    D_80082EB0 = D_80082EA8;
                }
                if (*p & 0x90) {
                    func_8005D6EC(3);
                    D_8009B308 = 0;
                    D_80082EA8 = D_80082EB0;
                }
                if (*p & 0x8000) {
                    s32 n, c;
                    func_8005D6EC(1);
                    c = D_80082EA8;
                    n = 0x13;
                    if (c > 0) n = c - 1;
                    D_80082EA8 = n;
                }
                if (D_801E436E & 0x2000) {
                    s32 n, c;
                    func_8005D6EC(1);
                    c = D_80082EA8;
                    n = 0;
                    if (c < 19) n = c + 1;
                    D_80082EA8 = n;
                }
            }
            t = D_80082EA8;
        }
        func_8004E368(1, t + 1);
        func_800489AC(D_8019C9F0, 2, D_8019C770);
        func_800487D8(&D_80081CA4, &D_8019C9F0, 0);
        func_800487D8(&D_80082460, &D_8019C9F0, 1);
        return;
    }

    D_8009B340 = -1;
    D_8009B344 = 8;
    func_8004E368(-1, 0);
    func_800487D8(&D_80081CA4, &D_8019C9F0, -1);
    func_800487D8(&D_80082460, &D_8019C9F0, 0);
    func_800489AC(D_8019C9F0, 2, D_8019C770);
    if (D_8019C9F0 <= 0) {
        D_8019C9F8 = 7;
        D_8009B340 = 7;
        D_8019C770 = 0;
        D_8019C9F0 = 0;
        D_8009B308 = 0;
    }
}

#include "common.h"

extern s32 D_8009B338;
extern s32 D_8019CB0C;
extern s32 D_8009B308;
extern s32 D_8009B318;
extern s32 D_8009B340;
extern s32 D_8009B344;
extern s32 D_8009B34C;
extern s32 D_8009B350;
extern s32 D_8009B358;
extern s32 D_8009B35C;
extern s32 D_8009B37C;
extern s32 D_8009B2F4;
extern s32 D_8009B2F8;
extern s32 D_8019C758;
extern s32 D_8019C9F0;
extern s32 D_8019C9F8;
extern s32 D_801E40D4;
extern u16 D_801E436E;
extern u8 D_8007F45C;

extern u8 D_800828EC;
extern u8 D_80082790;
extern u8 D_80081B54;
extern u8 D_80082460;

void func_8005131C(void);
s32 func_800487D8(void *a, void *b, s32 c);
void func_800489AC(s32 a, s32 b, s32 c);
void func_8005D6EC(s32 a);
void func_8004B8B4(s32 a, s32 b);
void func_8004A248(s32 a, s32 b);
s32 func_8004E724(s32 a, s32 b);

void func_80057198(void) {
    s32 sel;
    u16 edge;

    D_8019CB0C = D_8009B338;
    func_8005131C();
    if (D_8009B308 == 0) {
        func_800487D8(&D_800828EC, &D_8009B2F8, -1);
        func_800487D8(&D_80082790, &D_8009B2F8, 0);
        func_800489AC(D_8019C9F0, 3, D_8019C758);
        func_800487D8(&D_80081B54, &D_8019C9F0, 0);
        if (func_800487D8(&D_80082460, &D_8019C9F0, 1) != 0) {
            D_8009B318 = -1;
            if (D_801E436E & 0x1000) {
                func_8005D6EC(1);
                D_8019C758 = (D_8019C758 > 0) ? D_8019C758 - 1 : 3;
            }
            if (D_801E436E & 0x4000) {
                func_8005D6EC(1);
                D_8019C758 = (D_8019C758 < 3) ? D_8019C758 + 1 : 0;
            }
            if (D_801E436E & 0x860) {
                sel = D_8019C758;
                if (sel == 0) {
                    func_8005D6EC(2);
                    func_8004B8B4(-256, -256);
                    D_8009B308 = 1;
                    D_8009B318 = 1;
                } else if (sel == 1) {
                    func_8005D6EC(2);
                    D_8009B308 = 2;
                    D_8009B318 = sel;
                } else if (sel == 2) {
                    if (D_801E40D4 < 10) {
                        D_8009B308 = 3;
                        D_8009B318 = 1;
                        func_8005D6EC(2);
                    } else {
                        D_8009B308 = -1;
                        D_8009B2F8 = 0;
                        func_8005D6EC(5);
                    }
                } else if (sel == 3) {
                    func_8005D6EC(3);
                    D_8009B308 = 4;
                    D_8009B318 = 2;
                }
            } else if (D_801E436E & 0x90) {
                func_8005D6EC(3);
                D_8009B308 = 4;
                D_8009B318 = 2;
            }
        }
    } else if (D_8009B308 < 0) {
        func_800487D8(&D_800828EC, &D_8009B2F8, 0);
        if (func_800487D8(&D_80082790, &D_8009B2F8, 1) != 0) {
            edge = D_801E436E;
            if (edge & 0x860) D_8009B308 = 0;
            if (edge & 0x90) D_8009B308 = 0;
        }
        func_800489AC(D_8019C9F0, 3, D_8019C758);
        func_800487D8(&D_80081B54, &D_8019C9F0, 0);
        func_800487D8(&D_80082460, &D_8019C9F0, 1);
    } else {
        D_8009B340 = -1;
        D_8009B344 = 6;
        func_800487D8(&D_80081B54, &D_8019C9F0, -1);
        func_800487D8(&D_80082460, &D_8019C9F0, 0);
        func_800489AC(D_8019C9F0, 3, D_8019C758);
        if (D_8019C9F0 <= 0) {
            switch (D_8009B308) {
            case 1:
                D_8019C9F8 = 7;
                D_8009B340 = 7;
                func_8004A248(0, 0);
                break;
            case 2:
                D_8019C9F8 = 9;
                D_8009B340 = 9;
                func_8004E724(0, 0);
                D_8009B358 = 0x3D090;
                D_8009B35C = 0;
                D_8009B350 = 0;
                D_8009B34C = 0;
                D_8009B2F4 = (D_8007F45C >= 6) ? 0x2B : 0;
                D_8009B37C = D_8009B2F4;
                break;
            case 3:
                D_8019C9F8 = 10;
                D_8009B340 = 10;
                D_8009B2F8 = 0;
                D_8009B358 = 0x3D090;
                D_8009B35C = 0;
                break;
            case 4:
                D_8019C9F8 = 5;
                D_8009B340 = 5;
                D_8019C758 = 0;
                D_8009B358 = 0x3D090;
                D_8009B35C = 0;
                break;
            }
            D_8019C9F0 = 0;
            D_8009B308 = 0;
        }
    }
}

#include "common.h"
#include "game/car.h"

extern s32 D_8009B338;
extern s32 D_8019CB0C;
extern s32 D_8009B308;
extern s32 D_8009B2F8;
extern s32 D_8009B318;
extern s32 D_8009B340;
extern s32 D_8009B344;
extern s32 D_8009B35C;
extern s32 D_80082EB4;
extern u8 D_80082010;
extern u8 D_80082460;
extern s32 D_801F17A0;
extern s32 D_8019C9F0;
extern s32 D_8019C9F8;
extern GameCarEntry *D_8019C7C8;
extern u16 D_801E436E;
extern u16 D_801E4370;
extern s32 D_801E40D4;
extern GameCarEntry D_801E4388[];

void func_8005131C(void);
s32 func_8004F048(void *, s32, s32);
void func_80049418(s32, s32, s32, s32);
void func_800489AC(s32, s32, s32);
s32 func_800487D8(void *, void *, s32);
void func_8005D6EC(s32);
void func_8001D8C4(s32);
void func_8001DA74(s32);

void func_80058C14(void) {
    D_8019CB0C = D_8009B338;
    func_8005131C();

    if (D_8009B308 == 0) {
        func_8004F048(&D_8009B2F8, -1, D_80082EB4);
        func_80049418(-1, 0, 1, 1);
        func_800489AC(D_8019C9F0, 2, D_801F17A0);
        func_800487D8(&D_80082010, &D_8019C9F0, 0);
        if (func_800487D8(&D_80082460, &D_8019C9F0, 1) == 0) {
            return;
        }
        if (D_8009B2F8 > 0) {
            return;
        }
        D_8009B318 = -1;
        if (D_801E436E & 0x1000) {
            func_8005D6EC(1);
            D_801F17A0 = D_801F17A0 > 0 ? D_801F17A0 - 1 : 2;
        }
        if (D_801E436E & 0x4000) {
            func_8005D6EC(1);
            D_801F17A0 = D_801F17A0 < 2 ? D_801F17A0 + 1 : 0;
        }
        {
            u16 f = D_801E436E;
            if (f & 0x860) {
                s32 sel = D_801F17A0;
                s32 val;
                if (sel == 0) {
                    func_8005D6EC(2);
                    val = D_8019C7C8[D_801E40D4].shapeIndex;
                    D_8009B308 = -1;
                    D_8009B2F8 = 0;
                    D_80082EB4 = val;
                } else if (sel == 1) {
                    func_8005D6EC(2);
                    val = D_8019C7C8[D_801E40D4].textureIndex;
                    D_8009B308 = -2;
                    D_8009B2F8 = 0;
                    D_80082EB4 = val;
                } else if (sel == 2) {
                    func_8005D6EC(3);
                    D_8009B308 = 1;
                    D_8009B318 = 2;
                    D_8009B35C = 0x3D090;
                }
            } else if (f & 0x90) {
                func_8005D6EC(3);
                D_8009B308 = 3;
                D_8009B318 = 2;
                D_8009B35C = 0x3D090;
            }
        }
        return;
    }

    if (D_8009B308 < 0) {
        if (func_8004F048(&D_8009B2F8, 1, D_80082EB4) != 0) {
            if (D_801E4370 & 0x8000) {
                func_8005D6EC(1);
                D_80082EB4 = D_80082EB4 > 0 ? D_80082EB4 - 1 : 0x11;
            }
            if (D_801E4370 & 0x2000) {
                func_8005D6EC(1);
                D_80082EB4 = D_80082EB4 < 17 ? D_80082EB4 + 1 : 0;
            }
            if (D_8009B308 == -1) {
                u16 *btn = &D_801E436E;
                if (*btn & 0x860) {
                    func_8005D6EC(2);
                    D_8019C7C8[D_801E40D4].shapeIndex = D_80082EB4;
                    D_801E4388[D_801E40D4].shapeIndex = D_80082EB4;
                    D_801E4388[D_801E40D4].textureIndex = D_8019C7C8[D_801E40D4].textureIndex;
                    D_8009B308 = 0;
                }
                if (*btn & 0x90) {
                    func_8005D6EC(3);
                    D_80082EB4 = D_8019C7C8[D_801E40D4].shapeIndex;
                    D_8009B308 = 0;
                }
                func_8001D8C4(D_80082EB4);
            } else {
                u16 *btn = &D_801E436E;
                if (*btn & 0x860) {
                    func_8005D6EC(2);
                    D_8019C7C8[D_801E40D4].textureIndex = D_80082EB4;
                    D_801E4388[D_801E40D4].shapeIndex = D_8019C7C8[D_801E40D4].shapeIndex;
                    D_801E4388[D_801E40D4].textureIndex = D_80082EB4;
                    D_8009B308 = 0;
                }
                if (*btn & 0x90) {
                    func_8005D6EC(3);
                    D_80082EB4 = D_8019C7C8[D_801E40D4].textureIndex;
                    D_8009B308 = 0;
                }
                func_8001DA74(D_80082EB4);
            }
        }

        func_80049418(1, 0, 1, 1);
        func_800489AC(D_8019C9F0, 2, D_801F17A0);
        func_800487D8(&D_80082010, &D_8019C9F0, 0);
        func_800487D8(&D_80082460, &D_8019C9F0, 1);
        return;
    }

    D_8009B340 = -1;
    D_8009B344 = 10;
    func_800487D8(&D_80082010, &D_8019C9F0, -1);
    func_800487D8(&D_80082460, &D_8019C9F0, 0);
    func_800489AC(D_8019C9F0, 2, D_801F17A0);
    if (D_8019C9F0 <= 0) {
        D_8019C9F8 = 6;
        D_8009B340 = 6;
        D_801F17A0 = 0;
        D_8019C9F0 = 0;
        D_8009B308 = 0;
    }
}

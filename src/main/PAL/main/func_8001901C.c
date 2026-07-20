#include "common.h"
#include "game/asset.h"
#include "game/car.h"

extern u8 *D_8019CAFC;
extern u8 *D_801F17A8;
extern u8 *D_801E8AB0;
extern u8 *D_801E42D0;
extern s32 D_801E4D70;
extern u8 *D_8019C754;
extern GameCarEntry *D_8019C7C8;
extern s32 D_801E40D4;
extern s32 D_801E428C;
extern s32 D_8009E6A4;

void func_8005B768(s32 arg0, void *arg1, void *arg2, void *arg3);
s32 func_8005B89C(void);
s32 func_80017848(s32 arg0, s32 arg1);
s32 func_80017C78(s32 arg0, void *arg1);
void func_80034DF4(void);
void func_8001A3C0(void *arg0);
void func_8001A2E0(void *arg0);
void func_8001A40C(void *arg0);
void func_80019EBC(void);
void func_80017BD4(void *arg0);
void func_8004553C(void *arg0);
void func_8004550C(void *arg0);
void func_80017948(void *arg0, s32 arg1);
void func_8002A6B0(void *arg0);
void func_80017A6C(void *arg0);
void func_80017AD0(void *arg0);
void func_8004121C(void *arg0);
void func_80034E04(void *arg0);
void func_80043AC8(void *arg0, s32 arg1);
s32 func_80017C2C(void);

void func_8001901C(void) {
    switch (g_AssetLoadState) {
    case 1: {
        s32 *src = (s32 *)D_801F17A8;
        s32 raw = D_801E4D70;
        s32 *dst = (s32 *)D_8019CAFC;
        s32 n = raw / 4;
        while (n != 0) {
            *dst = *src;
            src++;
            n--;
            dst++;
        }
        func_8005B768(2, D_8019CAFC, D_801E8AB0, 0);
        g_AssetLoadState = 2;
        D_8019CAFC = D_8019CAFC + D_801E4D70;
        break;
    }
    case 2:
        if ((func_8005B89C() << 16) != 0) {
            g_AssetLoadState = 3;
        }
        break;
    case 3: {
        s32 idx = D_801E40D4;
        s32 sz = func_80017848(idx, D_8019C7C8[idx].modelVariant);
        if (func_80017C78((sz << 1) + 11, D_8019CAFC) != 0) {
            register u8 *base_a0 asm("$4");
            register u8 *base_a3 asm("$7");
            register u8 *p1 asm("$5");
            register u8 *p2 asm("$6");
            base_a0 = D_8019CAFC;
            D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 0);
            func_80034DF4();
            base_a3 = D_8019CAFC;
            p1 = base_a3 + *(s32 *)(base_a3 + 4);
            p2 = base_a3 + *(s32 *)(base_a3 + 12);
            base_a3 = base_a3 + *(s32 *)(base_a3 + 8);
            D_801F17A8 = p1;
            D_8019C754 = base_a3;
            D_801E8AB0 = p2;
            func_8005B768(3, p1, p2, base_a3);
            base_a0 = D_8019CAFC;
            D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 16);
            func_8001A3C0(D_801F17A8);
            g_AssetLoadState = 4;
            D_8019CAFC = D_801E8AB0;
        }
        break;
    }
    case 4:
        if ((func_8005B89C() << 16) != 0) {
            g_AssetLoadState = 5;
        }
        break;
    case 5: {
        register u8 *p asm("$5");
        register s32 scaled asm("$2");
        register s32 base_off asm("$4");
        p = D_8019CAFC;
        scaled = D_801E428C << 1;
        base_off = (D_8009E6A4 << 3) + 0x57;
        if (func_80017C78(scaled + base_off, p) != 0) {
            register u8 *base_a0 asm("$4");
            u8 *base;
            s32 off0, off1;
            base_a0 = D_8019CAFC;
            D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 0);
            func_8001A3C0(D_801F17A8);
            base_a0 = D_8019CAFC;
            D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 4);
            func_8001A3C0(D_801F17A8);
            base_a0 = D_8019CAFC;
            D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 8);
            func_8001A2E0(D_801F17A8);
            base = D_8019CAFC;
            off0 = *(s32 *)(base + 12);
            off1 = *(s32 *)(base + 16);
            D_801F17A8 = base + off0;
            D_801E8AB0 = base + off1;
            func_8001A3C0(D_801F17A8);
            func_8001A40C(D_8019CAFC);
            D_801E42D0 = D_8019CAFC;
            func_8001A3C0(D_801E8AB0);
            func_80019EBC();
            g_AssetLoadState = 6;
            D_8019CAFC = D_8019CAFC + 0x38000;
        }
        break;
    }
    case 6: {
        register u8 *p asm("$5");
        register s32 scaled asm("$3");
        register s32 result asm("$2");
        p = D_8019CAFC;
        scaled = D_801E428C << 1;
        result = (D_8009E6A4 << 3) + scaled;
        if (func_80017C78(result + 0x58, p) != 0) {
            register u8 *base_a0 asm("$4");
            base_a0 = D_8019CAFC; D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 0); func_80017BD4(D_801F17A8);
            base_a0 = D_8019CAFC; D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 4); func_8004553C(D_801F17A8);
            base_a0 = D_8019CAFC; D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 8); func_8004550C(D_801F17A8);
            base_a0 = D_8019CAFC; D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 12); func_80017948(D_801F17A8, 1);
            base_a0 = D_8019CAFC; D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 16); func_8002A6B0(D_801F17A8);
            base_a0 = D_8019CAFC; D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 20); func_80017A6C(D_801F17A8);
            base_a0 = D_8019CAFC; D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 24); func_80017948(D_801F17A8, 2);
            base_a0 = D_8019CAFC; D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 28); func_80017AD0(D_801F17A8);
            base_a0 = D_8019CAFC; D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 32); func_8004121C(D_801F17A8);
            base_a0 = D_8019CAFC; D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 36); func_80034E04(D_801F17A8);
            base_a0 = D_8019CAFC; D_801F17A8 = base_a0 + *(s32 *)(base_a0 + 40); func_80043AC8(D_801F17A8, 1);
            g_AssetLoadState = 7;
        }
        break;
    }
    case 7:
        if (func_80017C2C() != 0) {
            g_AssetLoadState = 0;
        }
        break;
    }
}

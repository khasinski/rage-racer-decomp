#include "common.h"

extern s32 D_8009B338, D_8019CB0C, D_8009B308, D_8009B2F4, D_8009B318;
extern s32 D_8009B380, D_8009B350, D_8009B34C, D_8009B35C, D_8009B340, D_8009B344, D_8009B358;
extern s32 D_8019C9F0, D_8019C9F8;
extern u8 D_8007F45C, D_8007F460[];
extern u16 D_801E4370, D_801E436E;
extern u32 D_80081D34;

void func_80051D6C(void);
s32 func_8004E724(s32 a, s32 b);
s32 func_800487D8(void *a, void *b, s32 c);
void func_8005D6EC(s32 a);
void func_8001D530(void *a, s32 b);

void func_8005873C(void) {
    u16 pad;
    s32 newdepth;

    D_8019CB0C = D_8009B338;
    func_80051D6C();
    if (D_8009B308 != 0) goto reopen;

    func_8004E724(1, D_8009B2F4);
    if (func_800487D8(&D_80081D34, &D_8019C9F0, 1) == 0) return;
    D_8009B318 = -1;

    if (D_8007F45C < 6) {
        if ((D_801E4370 & 0xF000) && D_8009B380 < 0) {
            if (D_801E4370 & 0x1000) { s32 u = D_8009B2F4; D_8009B2F4 = (u < 0xB) ? u + 0x21 : u - 0xB; }
            if (D_801E4370 & 0x4000) { s32 d = D_8009B2F4; D_8009B2F4 = (d < 0x21) ? d + 0xB : d - 0x21; }
            if (D_801E4370 & 0x8000) { s32 l = D_8009B2F4; D_8009B2F4 = (l % 11 != 0) ? l - 1 : l + 0xA; }
            if (D_801E4370 & 0x2000) {
                register s32 r asm("$5");
                register s32 res asm("$2");
                s32 rn;
                r = D_8009B2F4;
                rn = r + 1;
                if (rn % 11 == 0) res = r - 0xA; else res = rn;
                D_8009B2F4 = res;
            }
            D_8009B350 = 0;
            D_8009B34C = 0x3E8000;
            D_8009B380 = D_8009B2F4;
            func_8005D6EC(1);
            goto after_sound;
        }
        goto after_sound;
    }

    if (!((D_801E4370 & 0xA000) && D_8009B380 < 0)) goto after_sound;
    {
        s32 nc = (D_8009B2F4 == 0x2A) ? 0x2B : 0x2A;
        D_8009B2F4 = nc;
        D_8009B350 = 0;
        D_8009B34C = 0x3E8000;
        D_8009B380 = nc;
    }
    func_8005D6EC(1);
after_sound:
    pad = D_801E436E;
    if (!(pad & 0x860)) goto maybe_pop;
    {
        s32 c = D_8009B2F4;
        if (c == 0x2A) goto pop;
        if (c != 0x2B) goto push;
    }
    func_8005D6EC(3);
    D_8009B308 = 1;
    D_8009B318 = 2;
    D_8009B35C = 0x3D090;
    return;

push:
    {
        u32 d;
        func_8005D6EC(2);
        D_8007F460[D_8007F45C] = (u8)D_8009B2F4;
        d = D_8007F45C;
        if (d >= 5) D_8009B2F4 = 0x2B;
        if (d >= 7) newdepth = d; else newdepth = d + 1;
    }
    goto set_depth;

maybe_pop:
    if (!(pad & 0x90)) return;
pop:
    if (D_8007F45C == 0) return;
    func_8005D6EC(4);
    {
        register s32 tv asm("$2");
        tv = 0xA;
        D_8007F460[D_8007F45C] = tv;
    }
    newdepth = D_8007F45C - 1;
set_depth:
    D_8007F45C = newdepth;
    return;

reopen:
    D_8009B340 = -1;
    D_8009B344 = 9;
    func_8004E724(-1, D_8009B2F4);
    func_800487D8(&D_80081D34, &D_8019C9F0, -1);
    if (D_8019C9F0 > 0) return;
    if (0x3D08F < D_8009B358) {
        D_8019C9F8 = 6;
        D_8009B340 = 6;
        func_8001D530(&D_8007F460, D_8007F45C);
        D_8019C9F0 = 0;
        D_8009B308 = 0;
    }
}

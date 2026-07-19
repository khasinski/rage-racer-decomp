#include "common.h"

extern u16 D_800941EC[];
extern u16 D_800941EE[];
extern u32 D_8009B9B0[];

u32 func_8006764C(s32 arg0);
void func_80067600(void *arg0);

s32 func_80066E6C(s16 *env, u32 rgb) {
    {
        register s32 x asm("$3") = env[2];
        register s32 value asm("$2");
        if (x >= 0) {
            register volatile u16 *p asm("$2") = D_800941EC;
            register s32 mm asm("$4");
            value = *p;
            value = (s16)value;
            mm = value - 1;
            value = mm < x;
            if (value != 0) {
                x = mm;
            }
            value = x;
        } else {
            value = 0;
        }
        env[2] = value;
    }

    {
        register s32 x asm("$3") = env[3];
        if (x >= 0) {
            register volatile u16 *p asm("$2") = D_800941EE;
            register s32 mm asm("$4");
            register s32 c asm("$2");
            c = *p;
            c = (s16)c;
            mm = c - 1;
            c = mm < x;
            if (c != 0) {
                x = mm;
            }
        } else {
            x = 0;
        }
        env[3] = x;
    }

    if ((((u16 *)env)[0] & 0x3F) != 0 || (((u16 *)env)[2] & 0x3F) != 0) {
        u32 *p8 = &D_8009B9B0[8];
        u32 c3 = 0xE3000000;
        u32 c5 = 0xE5000000;
        D_8009B9B0[0] = ((u32)p8 & 0xFFFFFF) | 0x07000000;
        D_8009B9B0[1] = c3;
        D_8009B9B0[2] = 0xE4FFFFFF;
        D_8009B9B0[3] = c5;
        D_8009B9B0[4] = 0xE6000000;
        D_8009B9B0[5] = (rgb & 0xFFFFFF) | 0x60000000;
        D_8009B9B0[6] = ((u32 *)env)[0];
        D_8009B9B0[7] = ((u32 *)env)[1];
        *p8 = 0x03FFFFFF;
        D_8009B9B0[9] = func_8006764C(3) | c3;
        D_8009B9B0[10] = func_8006764C(4) | 0xE4000000;
        D_8009B9B0[11] = func_8006764C(5) | c5;
    } else {
        D_8009B9B0[0] = 0x04FFFFFF;
        D_8009B9B0[1] = 0xE6000000;
        D_8009B9B0[2] = (rgb & 0xFFFFFF) | 0x02000000;
        D_8009B9B0[3] = ((u32 *)env)[0];
        D_8009B9B0[4] = ((u32 *)env)[1];
    }

    func_80067600(&D_8009B9B0[0]);
    return 0;
}

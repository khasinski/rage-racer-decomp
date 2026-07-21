#include "common.h"

extern u8 D_8009E0C4[];
extern u8 D_8009E0C6[];
extern u8 D_8009E0CA[];
extern u8 D_8009E0CC[];
extern u8 D_8009E0CE[];
extern u8 D_8009DF24[];
extern u8 D_8009E0A0[];
extern u8 D_801E4BD7;
extern u8 *D_801E416C;
extern u16 D_801E4BEA;
extern u8 D_801E4BDC;

s32 func_80074A6C(s32 arg0, s32 arg1);

s32 func_80075CB0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg5) {
    register s32 raw asm("$10") = arg0;
    register s32 cst asm("$2") = 0xFFC0;
    register s32 a4 asm("$4");
    register s32 i asm("$4");
    s32 t1;
    s32 off;
    s32 w, t, rem, f0;
    register s32 prod asm("$4");
    register s32 q asm("$2");
    s32 bal;
    s32 base;
    s32 j, off2;
    u8 c;
    s32 ret;

    __asm__("" : "=r"(a4) : "0"(arg0), "r"(cst));
    i = (s16)a4;
    t1 = arg5 + cst;
    off = ((((i * 3) * 4) + i) * 4);

    if (*(s16 *)&D_8009E0C6[off] == (s16)arg1 &&
        *(s16 *)&D_8009E0CE[off] == (s16)arg2 &&
        *(s16 *)&D_8009E0CA[off] == (s16)arg3) {

        t = *(u16 *)&D_8009E0CC[off] + (D_801E4BD7 << 4);
        f0 = *(u16 *)&D_8009E0C4[off];
        w = (s16)t1;

        if (w > 0) {
            prod = w * (u8)D_801E416C[(((u16)t) << 5) + 0xD];
            q = prod / 63;
            base = f0 + q;
            bal = (prod - q * 63) << 1;
        } else if (w < 0) {
            prod = w * (u8)D_801E416C[(((u16)t) << 5) + 0xC];
            q = prod;
            if (prod < 0) q = prod + 0x3F;
            q = q >> 6;
            base = f0 + q - 1;
            {
                register s32 r asm("$2");
                r = prod - q * 64;
                bal = (r << 1) + 0x7F;
            }
        } else {
            base = f0;
            bal = 0;
        }

        j = (s16)raw;
        off2 = ((((j * 3) * 4) + j) * 4);
        c = *(u8 *)&D_8009E0CC[off2];
        D_801E4BEA = raw;
        D_801E4BDC = c;
        ret = func_80074A6C((u16)base, (u16)bal);
        *(s16 *)&D_8009DF24[j << 4] = ret;
        D_8009E0A0[j] |= 4;
        return 1;
    }
    return 0;
}

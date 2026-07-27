#include <sys/types.h>

#include "common.h"

extern u_char D_8009E0C4[];
extern u_char D_8009E0C6[];
extern u_char D_8009E0CA[];
extern u_char D_8009E0CC[];
extern u_char D_8009E0CE[];
extern u_char D_8009DF24[];
extern u_char D_8009E0A0[];
extern u_char D_801E4BD7;
extern u_char *D_801E416C;
extern u_short D_801E4BEA;
extern u_char D_801E4BDC;

long func_80074A6C(long arg0, long arg1);

long SpuVmApplyPitchBendToVoice(long arg0, long arg1, long arg2, long arg3, long arg5) asm("func_80075CB0");
long SpuVmApplyPitchBendToVoice(long arg0, long arg1, long arg2, long arg3, long arg5) {
    register long raw asm("$10") = arg0;
    register long cst asm("$2") = 0xFFC0;
    register long a4 asm("$4");
    register long i asm("$4");
    long t1;
    long off;
    long w, t, rem, f0;
    register long prod asm("$4");
    register long q asm("$2");
    long bal;
    long base;
    long j, off2;
    u_char c;
    long ret;

    __asm__("" : "=r"(a4) : "0"(arg0), "r"(cst));
    i = (short)a4;
    t1 = arg5 + cst;
    off = ((((i * 3) * 4) + i) * 4);

    if (*(short *)&D_8009E0C6[off] == (short)arg1 &&
        *(short *)&D_8009E0CE[off] == (short)arg2 &&
        *(short *)&D_8009E0CA[off] == (short)arg3) {

        t = *(u_short *)&D_8009E0CC[off] + (D_801E4BD7 << 4);
        f0 = *(u_short *)&D_8009E0C4[off];
        w = (short)t1;

        if (w > 0) {
            prod = w * (u_char)D_801E416C[(((u_short)t) << 5) + 0xD];
            q = prod / 63;
            base = f0 + q;
            bal = (prod - q * 63) << 1;
        } else if (w < 0) {
            prod = w * (u_char)D_801E416C[(((u_short)t) << 5) + 0xC];
            q = prod;
            if (prod < 0) q = prod + 0x3F;
            q = q >> 6;
            base = f0 + q - 1;
            {
                register long r asm("$2");
                r = prod - q * 64;
                bal = (r << 1) + 0x7F;
            }
        } else {
            base = f0;
            bal = 0;
        }

        j = (short)raw;
        off2 = ((((j * 3) * 4) + j) * 4);
        c = *(u_char *)&D_8009E0CC[off2];
        D_801E4BEA = raw;
        D_801E4BDC = c;
        ret = func_80074A6C((u_short)base, (u_short)bal);
        *(short *)&D_8009DF24[j << 4] = ret;
        D_8009E0A0[j] |= 4;
        return 1;
    }
    return 0;
}

#include <sys/types.h>

#include "common.h"

void SpuVmSeKeyOn(long arg0, short arg1, short arg2, u_short arg3, u_short arg4, u_short arg5) asm("func_80076350");
void SpuVmSeKeyOff(long arg0, short arg1, short arg2, u_short arg3) asm("func_80076940");

/* Empty two-word stub. Was misnamed SsUtKeyOnV, which is the eight
 * argument function at func_80077C7C in sdk/SsUtKeyOnV.c. */
void func_80076C50(void);

void func_80076B30(long arg0, long arg1, long arg2, long arg3, u_short arg4, u_short arg5) {
    u_short base;
    long balance;
    /* This pin is load-bearing: removing it changes .text. */
    register long quotient asm("$2");

    if (arg4 == arg5) {
        balance = 0x40;
        base = arg4;
    } else if (arg5 < arg4) {
        quotient = (arg5 << 6) / arg4;
        base = arg4;
        balance = quotient;
    } else {
        quotient = (arg4 << 6) / arg5;
        base = arg5;
        balance = 0x7F - quotient;
    }

    SpuVmSeKeyOn(0x21, (short)arg0, (short)arg1, (u_short)arg2, base, (u_short)balance);
}

void func_80076C1C(long arg0, long arg1, long arg2) {
    SpuVmSeKeyOff(0x21, arg0, arg1, arg2);
}

void func_80076C50(void) {
}

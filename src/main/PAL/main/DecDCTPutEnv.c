#include "common.h"

extern u32 D_80083060[];
extern u32 D_800830A0[];
extern u8 D_8008305C[];
extern u8 D_800830E0[];

void MDEC_in(volatile u32 *arg0, s32 arg1) asm("func_800641D0");

u32 *DecDCTPutEnv(u32 *arg0) asm("func_80063F08");
u32 *DecDCTPutEnv(u32 *arg0) {
    u32 *ret;
    register u32 *dst asm("$5");
    s32 i;

    ret = arg0;
    dst = D_80083060;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *arg0++;
    }

    dst = D_800830A0;
    arg0 = ret + 0x10;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *arg0++;
    }

    MDEC_in((volatile u32 *)D_8008305C, 0x20);
    MDEC_in((volatile u32 *)D_800830E0, 0x20);

    return ret;
}

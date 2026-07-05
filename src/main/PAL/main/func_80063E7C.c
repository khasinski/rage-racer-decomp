#include "common.h"

extern u32 D_80083060[];
extern u32 D_800830A0[];
extern u32 D_800830E4[];

u32 *func_80063E7C(u32 *arg0) {
    u32 *dst;
    u32 *src;
    s32 i;

    dst = arg0;
    src = D_80083060;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *src++;
    }

    dst = arg0 + 0x10;
    src = D_800830A0;
    for (i = 0xF; i != -1; i--) {
        *dst++ = *src++;
    }

    dst = arg0 + 0x20;
    src = D_800830E4;
    for (i = 0x1F; i != -1; i--) {
        *dst++ = *src++;
    }

    return arg0;
}

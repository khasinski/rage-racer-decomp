#include "common.h"

extern u32 D_80083060[];
extern u32 D_800830A0[];
extern u8 D_8008305C[];
extern u8 D_800830E0[];

void func_800641D0(volatile u32 *arg0, s32 arg1);

u32 *func_80063F08(u32 *arg0) {
    u32 *ret;
    u32 *dst;
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

    func_800641D0((volatile u32 *)D_8008305C, 0x20);
    func_800641D0((volatile u32 *)D_800830E0, 0x20);

    return ret;
}

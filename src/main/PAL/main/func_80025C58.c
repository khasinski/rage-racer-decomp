#include "common.h"

extern u8 *D_8019C900;
extern s32 D_801E4B84;
extern s32 D_8007D6B0;
extern s32 D_801E4404;

s32 func_80016EC4(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_8001720C(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(u8 *arg0, s32 arg1, s32 arg2);

void func_80025C58(void) {
    u8 *base;
    s32 arg4;
    s32 arg5;
    s32 temp;
    s32 product;
    s32 value;
    s32 next;

    base = D_8019C900 + 0xD0;
    next = *(s32 *)0x1F800000;
    temp = (D_801E4B84 == 0) ? 0x3FEC : 0x3FEF;
    arg4 = 0x14;
    arg5 = 0x10;

    next = func_80016EC4(base, next, 0x20, 0xC1, arg4, arg5, 0, 0, temp);
    temp = (D_801E4B84 == 1) ? 0x3FEC : 0x3FEF;
    next = func_80016EC4(base, next, 0x36, 0xC1, arg4, arg5, arg4, 0, temp);
    temp = (D_801E4B84 == 2) ? 0x3FEC : 0x3FEF;
    next = func_80016EC4(base, next, 0x4C, 0xC1, arg4, arg5, 0x28, 0, temp);

    if (D_8007D6B0 != 0) {
        D_8007D6B0--;
        temp = 0x10;
    } else {
        value = D_801E4404;
        product = value << 1;
        product += value;
        product <<= 2;
        temp = product + 0x1C;
    }

    next = func_80016EC4(base, next, 0x64, 0xC2, 0xBA, 0xC, 0, temp, 0x3FED);
    next = func_80016EC4(base, next, 0x62, 0xC0, 0xBE, 0x10, 0x3C, 0, 0x3FEE);
    next = func_8001720C(base, next, 0x14, 0xB8, 0x118, 0x20, 0, 0, 0);
    *(s32 *)0x1F800000 = func_80017390(base, next, 0xB);
}

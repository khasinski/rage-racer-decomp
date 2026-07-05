#include "psyq/spu.h"

extern volatile u32 D_8009A76C;
extern volatile u16 *D_8009AB7C;

s32 SpuGetKeyStatus(u32 arg0) {
    s32 voice = -1;
    s32 i = 0;
    u32 mask = 1;
    register u32 value asm("$3");
    register s32 ret asm("$2");

loop:
    if (arg0 & (mask << i)) {
        goto found;
    }
    i++;
    if (i < 24) {
        goto loop;
    }

postscan:
    if (voice != -1) {
        goto body;
    }
    goto ret_minus_one;

found:
    voice = i;
    goto postscan;

body:
    {
        register u32 offset asm("$3") = voice << 4;
        register volatile u16 *base asm("$2") = D_8009AB7C;
        register u32 flags asm("$4") = D_8009A76C;
        register u32 flag asm("$2");

        offset += (u32)base;
        flag = (1 << voice) & flags;
        asm("" : "=r"(flag) : "0"(flag));
        value = *(volatile u16 *)(offset + 0xC);
        if (flag != 0) {
            ret = 3;
            if (value == 0) {
                goto ret_value;
            }
            ret = 1;
            goto ret_value;
        }

        ret = value != 0 ? 2 : 0;
        goto ret_value;
    }

ret_minus_one:
    return -1;

ret_value:
    return ret;
}

#include "common.h"

extern s32 D_801E40B8;
extern s32 D_8019C8EC;
extern s32 D_8019C900;

s32 func_80016EC4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(s32 arg0, s32 arg1, s32 arg2);

void func_8001FB8C(void) {
    register volatile s32 *scratch asm("$17");
    register s32 base asm("$16");
    s32 next;

    if ((D_801E40B8 & 0x10) && (D_8019C8EC == 0)) {
        scratch = (volatile s32 *)0x1F800000;
        base = D_8019C900 + 0xCC;
        next = func_80016EC4(base, *scratch, 0x10, 0x10, 0x48, 0x10, 0, 0x68, 0x780D);
        *scratch = func_80017390(base, next, 9);
    }
}

#include "common.h"

extern s32 D_8007F604;
extern s32 D_8007F60C;

s32 func_8006A534(s32 arg0, s32 arg1);
s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);

INCLUDE_RODATA("asm/PAL/main/nonmatchings/main/func_800432A8", func_800432A8_rodata);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_800432A8", func_800432A8);

void func_80043494(void) {
    s32 state;
    s32 status;

    state = D_8007F60C;
    if (state == 1) {
        goto state_1;
    }
    if (state >= 2) {
        goto state_ge_2;
    }
    if (state == 0) {
        goto state_0;
    }
    goto done;

state_ge_2:
    if (state == 2) {
        goto state_2;
    }
    if (state == 3) {
        goto state_3;
    }
    goto done;

state_0:
    if (func_8006A534(1, 0) == 0) {
        goto done;
    }
    D_8007F60C = 1;
state_1:
    if (func_8006A5A4(3, 0, 0) == 0) {
        goto done;
    }
    D_8007F60C = 2;
    goto done;

state_2:
    status = func_8006A534(1, 0);
    if (status == state) {
        D_8007F60C = 3;
        goto done;
    }
    if (status == 5) {
        D_8007F60C = 1;
        goto done;
    }
    goto done;

state_3:
    D_8007F604 = -1;
    D_8007F60C = 0;

done:
    return;
}

#include "common.h"

extern char D_800127CC[];

void func_800632F0(void *dst, void *fmt, s32 arg0, s32 arg1);
void func_8005F304(void);
s32 func_80063280(void *device);
s32 func_8005F55C(void);

INCLUDE_RODATA("asm/PAL/main/nonmatchings/main/func_8005ECE0", func_8005ECE0_rodata);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005ECE0", func_8005ECE0);

s32 func_8005EF44(s32 arg0, s32 arg1) {
    char device[8];
    register s32 status asm("$3");

    func_800632F0(device, D_800127CC, arg0, arg1);
    func_8005F304();
    func_80063280(device);
    status = func_8005F55C();

    if (status != 1) {
        if (status == 3) {
            status = -1;
        } else {
            status = -3;
        }
    }

    return status;
}

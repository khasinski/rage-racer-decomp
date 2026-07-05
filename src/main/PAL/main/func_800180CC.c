#include "common.h"

extern s32 D_8007BED8;
extern s32 D_8007C704;

void func_80042C94(void);

INCLUDE_RODATA("asm/PAL/main/nonmatchings/main/func_800180CC", func_800180CC_rodata);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_800180CC", func_800180CC);

s32 func_800182D0(void) {
    register s32 state asm("$16");

    if (D_8007BED8 != 0) {
        return 1;
    }

    state = 2;
    if (D_8007C704 == state) {
        D_8007C704 = 0;
        return 0;
    }

    func_80042C94();
    D_8007C704 = state;
    D_8007BED8 = 1;
    return 1;
}

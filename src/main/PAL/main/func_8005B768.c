#include "common.h"

s32 func_800731CC(void);
s32 func_8005E600(s32 arg0);
s32 func_8005B948(s32 arg0);

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005B768);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005B89C);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005B948);

s32 func_8005B9CC(void) {
    func_800731CC();
    if (func_8005E600(1) == 0) {
        return 0;
    }
    if (func_8005B948(2) == 0) {
        return 0;
    }
    if (func_8005B948(3) == 0) {
        return 0;
    }
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005BA20);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005BB1C);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005BC14);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005BC80);

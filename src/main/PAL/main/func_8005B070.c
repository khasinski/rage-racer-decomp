#include "common.h"
#include "psyq/snd.h"

extern s32 D_801E6D84;
extern s32 D_801E6D88;

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B070", func_8005B070);

void func_8005B190(s32 left, s32 right) {
    if (left >= 0) {
        if (left >= 0x80) {
            left = 0x7F;
        }
    } else {
        left = 0;
    }

    if (right >= 0) {
        if (right >= 0x80) {
            right = 0x7F;
        }
    } else {
        right = 0;
    }

    D_801E6D84 = left;
    D_801E6D88 = right;
    SsUtSetReverbDepth((s16)left, (s16)right);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B070", func_8005B204);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B070", func_8005B2F0);

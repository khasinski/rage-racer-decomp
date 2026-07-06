#include "common.h"
#include "psyq/snd.h"

extern s32 D_801E6D84;
extern s32 D_801E6D88;
extern s32 D_801E6D80;

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

void func_8005B204(s32 type, s32 left, s32 right) {
    s32 tempLeft;
    s32 tempRight;

    if (left >= 0) {
        tempLeft = left;
        if (tempLeft >= 0x80) {
            tempLeft = 0x7F;
        }
    } else {
        tempLeft = 0;
    }

    if ((left = tempLeft, right) >= 0) {
        tempRight = right;
        if (tempRight >= 0x80) {
            tempRight = 0x7F;
        }
    } else {
        tempRight = 0;
    }
    right = tempRight;

    SsUtReverbOff();

    if ((u32)(type - 1) < 9) {
        D_801E6D80 = type;
        D_801E6D84 = left;
        D_801E6D88 = right;
        SsUtSetReverbType((s16)type);
        SsUtReverbOn();
        func_8005B190(left, right);
    } else {
        D_801E6D80 = 0;
        D_801E6D88 = 0;
        D_801E6D84 = 0;
    }
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B070", func_8005B2F0);

#include "common.h"
#include "psyq/snd.h"

extern s32 D_801E6D84;
extern s32 D_801E6D88;
extern s32 D_801E6D80;
extern s16 D_801E6CA8[];
extern s16 D_80082F28[];

void func_80077C7C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void GamePlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot) asm("func_8005B2F0");

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

void GamePlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot) {
    register s16 *base asm("$3") = D_80082F28;
    register s16 *row asm("$2");
    register s16 *entry asm("$3");

    row = (s16 *)((s32)base + (slot << 2));
    entry = (s16 *)((tone << 1) + (s32)row);
    func_80077C7C((s16)(slot + 0xE), D_801E6CA8[(s16)vabSlot], *entry, 0, 0x3C, 0, 0, 0);
}

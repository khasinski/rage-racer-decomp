#include "common.h"
#include "game/audio.h"

extern s16 D_801E6CA8;
extern s32 D_801E6CF0;
extern s32 D_801E6CF8;
extern s32 D_801E6CFC;

void func_80077C7C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void func_80078018(s32 arg0);

void func_8005C09C(s32 arg0) {
    func_80077C7C(0x14, D_801E6CA8, (s16)arg0, 0, 0x3C, 0, 0, 0);
}

void func_8005C0E4(void) {
    func_80078018(0x14);
}

void GameSetIndexedEffectVoice(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 >= -1) {
        if (arg0 >= 3) {
            arg0 = 2;
        }
    } else {
        arg0 = -1;
    }

    if (arg2 >= 0) {
        if (arg2 >= 0x80) {
            arg2 = 0x7F;
        }
    } else {
        arg2 = 0;
    }

    D_801E6CF0 = arg0;
    if (arg0 >= 0) {
        D_801E6CFC = arg2;
        D_801E6CF8 = arg1;
    }
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005C09C", func_8005C168);

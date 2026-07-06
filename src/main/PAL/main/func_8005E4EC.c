#include "common.h"

extern s16 D_801E6CA8[];
extern s16 D_8009B510[];

void func_80077C7C(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7);

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005E4EC", func_8005E4EC);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005E4EC", func_8005E600);

void func_8005E694(s32 voice, s32 unused, s16 vabSlot) {
    register s32 voiceOffset asm("$2") = voice * 4;
    register s32 vabOffset asm("$6") = (s16)vabSlot * 2;

    asm("" : "=r"(voiceOffset) : "0"(voiceOffset));
    func_80077C7C((s16)voice, *(s16 *)((s32)D_801E6CA8 + vabOffset), *(s16 *)((s32)D_8009B510 + voiceOffset), 0, 0x3C, 0, 0, 0);
}

#include "common.h"

extern s16 D_801E6CA8[];
extern s16 D_8009B510[];
extern s32 D_801E6C9C;
extern s16 D_801E6D90;

void func_80077C7C(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7);
void func_80073748(s32 arg0, s32 arg1);
void func_8007865C(s32 arg0);
void func_80071AC4(s32 arg0);
void func_80072B3C(s32 arg0);

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005E4EC", func_8005E4EC);

s32 func_8005E600(s32 slot) {
    register s32 slotReg asm("$17") = slot;
    register s32 bit asm("$3") = 1;
    register s32 *flagsPtr asm("$16") = &D_801E6C9C;
    register s32 flags asm("$5");
    register s32 zeroArg asm("$4") = 0;
    register s32 ret asm("$2");
    register s32 newFlags asm("$2");
    s32 offset;

    flags = *flagsPtr;
    bit <<= slotReg;
    if (bit & flags) {
        goto loaded;
    }

    ret = 0;
    goto done;

loaded:
    newFlags = bit ^ flags;
    *flagsPtr = newFlags;
    func_80073748(zeroArg, 0);
    func_8007865C(0);
    func_80071AC4(D_801E6D90);
    offset = slotReg * 2;
    asm("addu %0, %1, %0" : "=r"(offset) : "r"(flagsPtr), "0"(offset));
    func_80072B3C(*(s16 *)(offset + 0xC));
    ret = 1;
done:
    return ret;
}

void func_8005E694(s32 voice, s32 unused, s16 vabSlot) {
    register s32 voiceOffset asm("$2") = voice * 4;
    register s32 vabOffset asm("$6") = (s16)vabSlot * 2;

    asm("" : "=r"(voiceOffset) : "0"(voiceOffset));
    func_80077C7C((s16)voice, *(s16 *)((s32)D_801E6CA8 + vabOffset), *(s16 *)((s32)D_8009B510 + voiceOffset), 0, 0x3C, 0, 0, 0);
}

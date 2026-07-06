#include "common.h"

extern s16 D_801E6CA8[];
extern s16 D_8009B510[];
extern s32 D_800125EC[];
extern s32 D_8009E68C;
extern s32 D_801E6C9C;
extern s16 D_801E6D90;
extern s32 D_801E6D9C;
extern s32 D_801F17B4;
extern char D_80012788[];
extern char D_800127A0[];

void func_8001674C(char *arg0);
void func_80063D9C(s32 arg0);
s32 func_8006F004(s32 arg0);
void func_80077C7C(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7);
s32 func_8007317C(s32 arg0);
s32 func_800730BC(s32 arg0, s32 arg1);
s32 func_80072C4C(s32 arg0, s32 arg1, s32 arg2);
void func_80073748(s32 arg0, s32 arg1);
void func_8007865C(s32 arg0);
void func_80071AC4(s32 arg0);
void func_80072B3C(s32 arg0);

s32 func_8005E4EC(s32 slot, s32 header, s32 body, s32 seq) {
    register s32 slotReg asm("$16") = slot;
    register s32 bodyReg asm("$18") = body;
    register s32 seqReg asm("$19") = seq;
    register s16 *vabIdPtr asm("$16");
    register s32 currentVabId asm("$5");
    register s32 fail asm("$17");
    register s32 ret asm("$2");

    D_8009E68C = slotReg;
    ret = func_80072C4C(header, -1, D_800125EC[slotReg]);
    {
        register s16 *vabIdBase asm("$3") = D_801E6CA8;
        slotReg *= 2;
        vabIdPtr = (s16 *)((s32)vabIdBase + slotReg);
    }
    *vabIdPtr = ret;
    asm volatile("" : "=r"(ret) : "0"(ret));

    currentVabId = (s16)ret;
    fail = -1;
    if (currentVabId == fail) {
        func_8001674C(D_80012788);
        func_80063D9C(1);
    }

    ret = func_800730BC(bodyReg, currentVabId);
    *vabIdPtr = ret;
    currentVabId = (s16)ret;
    if (currentVabId == fail) {
        func_8001674C(D_800127A0);
        func_80063D9C(1);
    }

    *(s32 *)&D_801E6D90 = (s16)func_8006F004(seqReg);
    D_801E6D9C = 0;
    ret = func_8007317C(0);
    D_801F17B4 = (s16)ret;
    return D_801F17B4;
}

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

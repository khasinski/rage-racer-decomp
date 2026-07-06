#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"

s32 func_800731CC(void);
s32 func_8007317C(s32 arg0);
s32 func_800730BC(s32 arg0, s32 arg1);
s32 func_80072C4C(s32 arg0, s32 arg1, s32 arg2);
s32 func_8005E600(s32 arg0);
s32 func_8005B948(s32 arg0);
void func_8001674C(char *arg0);
void func_80063D9C(s32 arg0);
void func_800736E8(void);
void func_80073614(s32 arg0);
void func_80073748(s32 arg0, s32 arg1);
void func_8007865C(s32 arg0);
void func_80078018(s32 arg0);
void func_8006DD30(s32 arg0);
void func_80072B3C(s32 arg0);
void func_80072260(void);
void func_80071C24(void);

extern s32 D_801E6C9C;
extern s32 D_801E6CA0;
extern s16 D_801E6CB0;
extern s16 D_801E6CB2;
extern s16 D_801E6CAE;
extern s32 D_801E6CC0;
extern s32 D_8009E68C;
extern s32 D_801F17B4;
extern s32 D_800125F8;
extern char D_8001267C[];
extern char D_80012694[];

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005B768);

s32 func_8005B89C(void) {
    s32 completed;
    register s32 *flagsPtr asm("$4");
    register s32 slot asm("$5");
    register s32 one asm("$6");
    register s32 value asm("$3");
    s32 bit;

    completed = func_8007317C(0);
    D_801F17B4 = (s16)completed;

    if ((s16)completed != 0) {
        flagsPtr = &D_801E6C9C;
        one = 1;
        slot = D_8009E68C;
        value = *flagsPtr;
        bit = (s16)(one << slot);
        *flagsPtr = bit | value;

        if (slot == 0) {
            D_801E6CA0 = one;
        } else if (slot == one) {
            D_801E6CA0 = slot;
        } else {
            value = 2;
            if ((slot == value) || (slot == 3)) {
                D_801E6CA0 = value;
            }
        }
    }

    return (s16)D_801F17B4;
}

s32 func_8005B948(s32 slot) {
    register s32 slotReg asm("$17") = slot;
    register s32 *flagsPtr asm("$16") = &D_801E6C9C;
    register s32 bit asm("$3") = 1;
    register s32 flags asm("$5") = *flagsPtr;
    register s32 zeroArg asm("$4") = 0;
    register s32 ret asm("$2");

    bit <<= slotReg;

    if (bit & flags) {
        goto loaded;
    }

    ret = 0;
    goto done;

loaded:
    {
        register s32 newFlags asm("$2") = bit ^ flags;
        *flagsPtr = newFlags;
        func_80073748(zeroArg, 0);
        func_8007865C(0);
        {
            s32 offset = slotReg * 2;
            // Preserve operand order for the matching address calculation.
            asm("addu %0, %1, %0" : "=r"(offset) : "r"(flagsPtr), "0"(offset));
            func_80072B3C(*(s16 *)(offset + 0xC));
        }
        ret = 1;
    }
done:
    return ret;
}

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

s32 func_8005BA20(s32 header, s32 body, u16 *table) {
    register s32 ret asm("$2");
    register s32 currentVabId asm("$5");
    register s16 *vabIdPtr asm("$18");
    register s32 fail asm("$16");
    register s32 tableReg asm("$19") = (s32)table;

    D_8009E68C = 3;
    ret = func_80072C4C(header, -1, D_800125F8);
    vabIdPtr = &D_801E6CAE;
    *vabIdPtr = ret;
    asm volatile("" : "=r"(ret) : "0"(ret));

    currentVabId = (s16)ret;
    fail = -1;
    if (currentVabId == fail) {
        func_8001674C(D_8001267C);
        func_80063D9C(1);
    }

    ret = func_800730BC(body, currentVabId);
    *vabIdPtr = ret;
    if ((s16)ret == fail) {
        func_8001674C(D_80012694);
        func_80063D9C(1);
    }

    if (tableReg != 0) {
        GameLoadAudioParameterTable((u16 *)tableReg);
    }

    D_801E6CC0 = 1;
    ret = func_8007317C(0);
    D_801F17B4 = (s16)ret;
    return D_801F17B4;
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005BB1C);

void func_8005BC14(void) {
    register s32 liveSlot asm("$16");
    register s32 *flagsPtr asm("$4") = &D_801E6C9C;
    register s32 flags asm("$3") = *flagsPtr;
    register s32 newFlags asm("$2");

    if (flags & 0x20) {
        newFlags = flags ^ 0x20;
        *flagsPtr = newFlags;
        func_800736E8();
        func_80073748(0x28, 0x28);
        func_80078018((s16)liveSlot);
        func_80072B3C(D_801E6CB2);
    }
}

void func_8005BC80(void) {
    register s32 i asm("$16");
    register s32 *flag asm("$3") = &D_801E6C9C;

    asm volatile("" : "=r"(flag) : "0"(flag));
    if (*flag != 0) {
        *flag = 0;
        func_800736E8();
        func_80073614(0);
        func_80073748(0, 0);
        i = 0;
        while (i < 24) {
            func_80078018((s16)i);
            i++;
        }
        func_8006DD30(2);
        func_80072B3C(D_801E6CB0);
        func_80072B3C(D_801E6CB2);
        func_80072260();
        func_80071C24();
    }
}

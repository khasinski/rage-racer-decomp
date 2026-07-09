#include "common.h"

extern s32 D_80082F44;
extern s32 D_801E4D90;
extern s32 D_801E4D94;
extern s32 D_801E6CA4;
extern s32 D_80011C8C[][6];

s32 func_80077C7C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

s32 func_8005D414(s32 cue, s32 volume) {
    register s32 savedCue asm("$16");
    register s32 *handle asm("$17");
    register s32 value asm("$2");
    register s32 result asm("$3");
    register s32 offset asm("$8");
    register s32 tone asm("$6");
    register s32 pitch asm("$7");
    register s32 scaled asm("$4");
    register s32 scaleBase asm("$5");
    register s32 callType asm("$4");

    savedCue = cue;
    asm("" : "=r"(savedCue) : "0"(savedCue));
    value = D_80082F44;
    result = -1;
    handle = &D_801E4D90;
    D_801E4D94 = result;
    *handle = result;

    if (value != savedCue) {
        value = ((savedCue * 3) << 3);
        scaled = volume * *(s32 *)((s32)D_80011C8C + value);
        offset = *(s32 *)((s32)D_80011C8C + value + 4);
        asm("" : "=r"(offset) : "0"(offset));
        tone = *(s32 *)((s32)D_80011C8C + value + 8);
        pitch = *(s32 *)((s32)D_80011C8C + value + 0xC);

        if (scaled < 0) {
            scaled += 0x7F;
        }

        scaleBase = (s32)&D_801E6CA4;
        value = scaled >> 7;
        result = value * *(s32 *)scaleBase;
        if (result < 0) {
            result += 0x7F;
        }

        value = offset << 1;
        asm("addu %0,%2,%0" : "=r"(value) : "0"(value), "r"(scaleBase));
        scaleBase = *(s16 *)(value + 4);
        asm("" : "=r"(scaleBase) : "0"(scaleBase));
        result = (s16)func_80077C7C(callType, scaleBase, tone, pitch, (value = 0x3C), ({
            value = (result << 9) >> 16;
            asm volatile(
                "sll %0,%0,16\n\t"
                "sll %1,%1,16\n\t"
                "li %2,0x00000013\n\t"
                "sra %0,%0,16\n\t"
                "sra %1,%1,16"
                : "=r"(tone), "=r"(pitch), "=r"(callType)
                : "0"(tone), "1"(pitch));
            0;
        }), value, value);
        *handle = result;
    }

    D_80082F44 = savedCue;
    return result;
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005D414", func_8005D530);

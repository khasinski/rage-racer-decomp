#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"

extern s32 D_801E6CA4;
extern s16 D_801E6CA8;
extern u8 D_801E6D00[];
extern u8 D_801E6D10[];

void func_80077C7C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void func_80078018(s32 arg0);

void func_8005DEF0(s32 enabled) {
    register s32 offset asm("$16");
    register s32 voicePacked asm("$17");
    register s32 voice asm("$18");
    register s32 i asm("$19");
    register s32 unused asm("$21");
    register s32 raw asm("$2");
    register s32 scale asm("$3");
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 arg0 asm("$4");
    register s32 zeroArg asm("$7");

    unused = 0;
    i = 0;
    voicePacked = 0x80000;
    voice = 8;
    offset = 0;
    do {
        if (enabled != 0) {
            arg0 = voicePacked >> 16;
            raw = 0x3C;
            left = D_801E6CA8;
            right = *(s16 *)(D_801E6D00 + offset);
            zeroArg = 0;
            func_80077C7C(arg0, left, right, zeroArg, raw, 0, 0, 0);
            asm volatile("" : : "r"(unused));

            raw = *(s32 *)(D_801E6D10 + offset);
            scale = D_801E6CA4;
            left = raw * scale;
            raw = *(s32 *)(D_801E6D10 + offset + 4);
            arg0 = voice;
            if (left < 0) {
                left += 0x7F;
            }
            raw *= scale;
            if (raw < 0) {
                raw += 0x7F;
            }
            left >>= 7;
            right = raw >> 7;

            if (left >= 0) {
                if (left >= 0x81) {
                    left = 0x80;
                }
            } else {
                left = 0;
            }

            if (right >= 0) {
                if (right >= 0x81) {
                    right = 0x80;
                }
            } else {
                right = 0;
            }

            SsUtSetVVol((s16)arg0, left, right);
        } else {
            func_80078018(voicePacked >> 16);
        }

        voicePacked += 0x10000;
        voice++;
        i++;
        offset += 0x18;
    } while (i < 2);
}
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005DEF0", func_8005E058);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005DEF0", func_8005E1D0);

#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"

extern s32 D_801E6CA4;
extern s16 D_801E6CA8;
extern s32 D_801E6CF4;
extern s32 D_801E6CF8;
extern s32 D_801E6CFC;
extern u8 D_801E6D00[];
extern u8 D_801E6D10[];
extern s32 D_800126AC[];
extern s32 D_800126B4[];

void func_80077C7C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void func_80078018(s32 arg0);
void func_8005C09C(s32 arg0);
void func_8005C0E4(void);

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

void func_8005E058(s32 enabled) {
    register s32 base asm("$16");
    register s32 center asm("$17");
    register s32 fine asm("$18");
    register s32 index asm("$3");
    register s32 raw asm("$2");
    register s32 product asm("$4");
    register s32 scale asm("$3");
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 voice asm("$4");

    if (enabled != 0) {
        index = D_801E6CF4;
        if (index < 0) {
            return;
        }
        raw = (index * 3) << 2;
        func_8005C09C(*(s32 *)((s32)D_800126AC + raw));
    } else {
        func_8005C0E4();
    }

    raw = D_801E6CF4;
    if (raw >= 0) {
        index = (raw * 3) << 2;
        product = D_801E6CFC * *(s32 *)((s32)D_800126B4 + index);
        raw = D_801E6CF8;
        base = *(s32 *)((s32)D_800126AC + index);
        center = raw >> 7;
        asm volatile("" : : "r"(center));
        fine = raw & 0x7F;
        if (product < 0) {
            product += 0x7F;
        }
        raw = product >> 7;
        scale = D_801E6CA4;
        raw *= scale;
        left = raw;
        if (raw < 0) {
            left = raw + 0x7F;
        }
        left >>= 7;
        right = left;

        if (right >= 0) {
            if (right >= 0x81) {
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

        SsUtSetVVol(0x14, left, right);
        voice = 0x14;
        left = 0;
        asm volatile("" : : "r"(voice), "r"(left));
        right = (s16)base;
        raw = (s16)center;
        SsUtChangePitch(voice, left, right, 0x3C, 0, raw, fine);
    }
}
asm(".globl func_8005E078\nfunc_8005E078 = func_8005E058 + 0x20");
asm(".globl func_8005E1B8\nfunc_8005E1B8 = func_8005E058 + 0x160");
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005DEF0", func_8005E1D0);

#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"

extern s32 D_801E6CA4;
extern s16 D_801E6CA8;
extern s32 D_801E6CE4;
extern s32 D_801E6CE8;

void func_80077C7C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void func_80078018(s32 arg0);

void func_8005DDB8(s32 enabled) {
    s32 values[2];
    register s32 i asm("$7");
    register s32 *src asm("$6");
    register s32 *dst asm("$5");
    register s32 scale asm("$3");
    register s32 raw asm("$2");
    register s32 voice asm("$4");
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 zeroArg asm("$7");
    register s32 unused asm("$16");

    i = 0;
    dst = values;
    src = &D_801E6CE4;
    do {
        if (*src < 2) {
            *dst = 0;
        } else {
            *dst = *src;
        }
        dst++;
        i++;
        src++;
    } while (i < 2);

    if (enabled != 0) {
        raw = values[0];
        scale = D_801E6CA4;
        left = raw * scale;
        raw = values[1];
        if (left < 0) {
            left += 0x7F;
        }
        unused = 0;
        asm volatile("" : : "r"(unused));
        raw *= scale;
        left >>= 7;
        if (raw < 0) {
            raw += 0x7F;
        }
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

        SsUtSetVVol(0x15, left, right);
        voice = 0x15;
        right = 0xF;
        asm volatile("" : : "r"(voice), "r"(right));
        raw = 0x3C;
        left = D_801E6CA8;
        zeroArg = 0;
        func_80077C7C(voice, left, right, zeroArg, raw, 0, 0, 0);
    } else {
        func_80078018(0x15);
    }
}

asm(".globl func_8005DEE4\nfunc_8005DEE4 = func_8005DDB8 + 0x12C");

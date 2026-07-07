#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"

extern s32 D_801E6CA4;
extern s16 D_801E6CA8;
extern s32 D_801E6CE4;
extern s32 D_801E6CEC;

void func_80077C7C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void func_80078018(s32 arg0);

void func_8005BF30(void) {
    s32 values[2];
    register s32 changed asm("$16");
    register s32 i asm("$6");
    register s32 *dst asm("$4");
    register s32 *src asm("$5");
    register s32 raw asm("$2");
    register s32 loopValue asm("$3");
    register s32 scale asm("$3");
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 voice asm("$4");
    register s32 zeroArg asm("$7");

    changed = 0;
    i = 0;
    dst = values;
    src = &D_801E6CE4;
    do {
        loopValue = *src;
        if (loopValue < 2) {
            *dst = 0;
        } else {
            *dst = loopValue;
            changed = 1;
        }
        dst++;
        i++;
        src++;
    } while (i < 2);

    if (changed != 0) {
        raw = values[0];
        scale = D_801E6CA4;
        left = raw * scale;
        raw = values[1];
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

        SsUtSetVVol(0x15, left, right);
        if (D_801E6CEC == 0) {
            right = 0xF;
            asm volatile("" : : "r"(right));
            voice = 0x15;
            asm volatile("" : : "r"(voice));
            raw = 0x3C;
            left = D_801E6CA8;
            zeroArg = 0;
            func_80077C7C(voice, left, right, zeroArg, raw, 0, 0, 0);
        }
    } else if (D_801E6CEC != 0) {
        func_80078018(0x15);
    }

    D_801E6CEC = changed;
}

asm(".globl func_8005C030\nfunc_8005C030 = func_8005BF30 + 0x100");

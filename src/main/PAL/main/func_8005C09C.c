#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"

extern s32 D_801E6CA4;
extern s16 D_801E6CA8;
extern s32 D_801E6CF0;
extern s32 D_801E6CF4;
extern s32 D_801E6CF8;
extern s32 D_801E6CFC;
extern s32 D_800126AC[];
extern s32 D_800126B4[];

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

void GameUpdateIndexedEffectVoice(void) {
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

    raw = D_801E6CF4;
    if (raw < 0) {
        index = D_801E6CF0;
        if (index < 0) {
            goto update_state;
        }
        goto start_voice;
    } else {
        index = D_801E6CF0;
        if (index < 0) {
            func_8005C0E4();
        } else if (index != raw) {
        start_voice:
            raw = (index * 3) << 2;
            func_8005C09C(*(s32 *)((s32)D_800126AC + raw));
        }
    }

    raw = D_801E6CF0;
    if (raw >= 0) {
        index = (raw * 3) << 2;
        product = D_801E6CFC * *(s32 *)((s32)D_800126B4 + index);
        raw = D_801E6CF8;
        base = *(s32 *)((s32)D_800126AC + index);
        center = raw >> 7;
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

update_state:
    D_801E6CF4 = D_801E6CF0;
}

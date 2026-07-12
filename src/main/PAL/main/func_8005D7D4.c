#include "common.h"

extern s32 D_801E6CA4;
extern s16 D_801E6CA8[];
extern s16 D_80082F28[][2];

void func_80078528(s32 voice, s16 left, s16 right);
void func_80078130(s32 voice, s32 vab_id, s32 program, s32 tone, s16 bend);

void func_8005D7D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) {
    register s32 voice asm("$4");
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 prod asm("$2");
    s32 bend;
    s32 voiceCopy;

    prod = arg2 * D_801E6CA4;
    voice = arg0 + 0xE;
    voiceCopy = voice;
    bend = arg4;
    left = prod;
    if (prod < 0) {
        left = prod + 0x7F;
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
    func_80078528((s16)voiceCopy, left, right);
    voice = arg0 + 0xE;
    func_80078130((s16)voice, D_801E6CA8[(s16)bend], D_80082F28[arg0][arg3], 0x3C, arg1);
}

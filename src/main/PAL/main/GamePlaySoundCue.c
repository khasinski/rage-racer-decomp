#include "common.h"
#include "game/audio.h"

extern s32 D_801E6CA0;
extern s32 D_80082F48;

void func_8005D050(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8005D530(s32 arg0, s32 arg1, s32 arg2);

void GamePlaySoundCue(s32 arg0) {
    s32 test;

    if (D_801E6CA0 == 1) {
        if (arg0 >= 0) {
            test = arg0 < 0x1E;
            if (test != 0) {
                goto mode1_clamped;
            }
            arg0 = 0x1D;
        } else {
            arg0 = 0;
        }

mode1_clamped:
        test = arg0 - 0xF;
        if ((u32)test < 3U) {
            goto special;
        }
        goto middle;
    }

    if (D_801E6CA0 == 2) {
        if (arg0 >= 0) {
            test = arg0 < 0x46;
            if (test != 0) {
                goto mode2_clamped;
            }
            arg0 = 0x45;
        } else {
            arg0 = 0;
        }

mode2_clamped:
        test = arg0 - 0xF;
        if ((u32)test < 3U) {
special:
            if (arg0 != D_80082F48) {
                D_80082F48 = arg0;
                GameStartSingleSpecialCue(arg0, 0x80);
            }
            return;
        }
        test = arg0 < 0x19;
        if (test == 0) {
            goto high;
        }
middle:
        func_8005D050(arg0, 0x3C, 0x80, 0x80);
        return;
high:
        func_8005D530(arg0, 0x80, 0x80);
    }
}

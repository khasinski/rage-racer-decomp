#include "common.h"

/*
 * The scripted-camera path: a table of 0x20-byte keyframes, each holding six
 * s32 (eye xyz then look-at xyz), a duration at +0x18 and a Bezier control
 * value at +0x1C. The player walks g_CameraPathKey -> g_CameraPathNextKey
 * over g_CameraPathTick frames and wraps to key 0 when a duration is -1.
 */
extern volatile s32 g_CameraPathKey asm("D_8007F61C");
extern s32 g_CameraPathNextKey asm("D_8007F620");
extern s32 g_CameraPathTick asm("D_8007F624");
extern s32 g_CameraPath asm("D_8007F628");

s32 GameBezierEase(s32 t, s32 control) asm("func_80046598");
s32 GameSetLookAtMatrix(s32 *obj) asm("func_80046248");

/* One frame of the scripted camera: eases the six values between the
 * current and next keyframe and installs the resulting view matrix. */
void GameUpdateScriptedCamera(void) asm("func_80046600");
void GameUpdateScriptedCamera(void) {
    s32 current;
    s32 tick;
    s32 currentOffset;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 nextOffset asm("$5");
    register s32 currentValue asm("$7");
    s32 scale;
    s32 blend;
    s32 scaledTick;
    s32 values[8];

    tick = g_CameraPathTick;
    current = g_CameraPathKey;
    scaledTick = tick * 10000;
    blend = GameBezierEase(scaledTick / *(s32 *)((s32)&g_CameraPath + (current << 5) + 0x18),
                          *(s32 *)((s32)&g_CameraPath + (current << 5) + 0x1C));

    currentOffset = g_CameraPathKey;
    nextOffset = g_CameraPathNextKey;
    currentOffset <<= 5;
    nextOffset <<= 5;
    scale = 0x68DB8BAD;

    tick = *(s32 *)((s32)&g_CameraPath + nextOffset);
    currentValue = *(s32 *)((s32)&g_CameraPath + currentOffset);
    values[0] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = *(s32 *)((s32)&g_CameraPath + nextOffset + 4);
    currentValue = *(s32 *)((s32)&g_CameraPath + currentOffset + 4);
    values[1] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = *(s32 *)((s32)&g_CameraPath + nextOffset + 8);
    currentValue = *(s32 *)((s32)&g_CameraPath + currentOffset + 8);
    values[2] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = *(s32 *)((s32)&g_CameraPath + nextOffset + 12);
    currentValue = *(s32 *)((s32)&g_CameraPath + currentOffset + 12);
    values[3] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = *(s32 *)((s32)&g_CameraPath + nextOffset + 16);
    currentValue = *(s32 *)((s32)&g_CameraPath + currentOffset + 16);
    values[4] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = *(s32 *)((s32)&g_CameraPath + nextOffset + 20);
    nextOffset = *(s32 *)((s32)&g_CameraPath + currentOffset + 20);
    values[5] = (((tick - nextOffset) * blend) / 10000) + nextOffset;
    GameSetLookAtMatrix(values);

    {
        s32 tailCurrent;
        /* These pins are load-bearing: removing any one changes .text. */
        register s32 tailTick asm("$2");
        register s32 tailNext asm("$4");

        tailCurrent = g_CameraPathKey;
        tailTick = g_CameraPathTick;
        tailCurrent = *(s32 *)((s32)&g_CameraPath + (tailCurrent << 5) + 0x18);
        tailTick++;
        tailTick = (tailTick < tailCurrent) ? tailTick : 0;
        g_CameraPathTick = tailTick;
        if (tailTick == 0) {
            tailNext = g_CameraPathNextKey;
            tailCurrent = tailNext + 1;
            tailTick = *(s32 *)((s32)&g_CameraPath + (tailCurrent << 5) + 0x18);
            g_CameraPathKey = tailNext;
            tailTick = ~tailTick;
            tailTick = (tailTick != 0);
            tailTick = -tailTick;
            tailCurrent &= tailTick;
            g_CameraPathNextKey = tailCurrent;
        }
    }
}

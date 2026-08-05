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
/* One scripted-camera keyframe: eye position, look-at target, how long the
 * player dwells on it and the Bezier control value used to ease out of it. */
typedef struct CameraKey {
    s32 eyeX;
    s32 eyeY;
    s32 eyeZ;
    s32 atX;
    s32 atY;
    s32 atZ;
    s32 duration;
    s32 control;
} CameraKey;

extern CameraKey g_CameraPath[] asm("D_8007F628");

/* The retail code re-derives the keyframe address at every field access rather
 * than holding a pointer, so this stays a macro: a real pointer variable lets
 * the compiler compute the base once and drops 20 instructions. */
#define KEY(byteOffset) (*(CameraKey *)((u8 *)g_CameraPath + (byteOffset)))

s32 BezierEase(s32 t, s32 control);
s32 SetLookAtMatrix(s32 *obj);

/* One frame of the scripted camera: eases the six values between the
 * current and next keyframe and installs the resulting view matrix. */
void UpdateScriptedCamera(void);
void UpdateScriptedCamera(void) {
    s32 current;
    s32 tick;
    s32 currentOffset;
    register s32 nextOffset asm("$5");
    register s32 currentValue asm("$7");
    s32 scale;
    s32 blend;
    s32 scaledTick;
    s32 values[8];

    tick = g_CameraPathTick;
    current = g_CameraPathKey;
    scaledTick = tick * 10000;
    blend = BezierEase(scaledTick / KEY(current * 32).duration,
                           KEY(current << 5).control);

    currentOffset = g_CameraPathKey;
    nextOffset = g_CameraPathNextKey;
    currentOffset <<= 5;
    nextOffset <<= 5;
    scale = 0x68DB8BAD;

    tick = KEY(nextOffset).eyeX;
    currentValue = KEY(currentOffset).eyeX;
    values[0] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = KEY(nextOffset).eyeY;
    currentValue = KEY(currentOffset).eyeY;
    values[1] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = KEY(nextOffset).eyeZ;
    currentValue = KEY(currentOffset).eyeZ;
    values[2] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = KEY(nextOffset).atX;
    currentValue = KEY(currentOffset).atX;
    values[3] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = KEY(nextOffset).atY;
    currentValue = KEY(currentOffset).atY;
    values[4] = (((tick - currentValue) * blend) / 10000) + currentValue;
    tick = KEY(nextOffset).atZ;
    nextOffset = KEY(currentOffset).atZ;
    values[5] = (((tick - nextOffset) * blend) / 10000) + nextOffset;
    SetLookAtMatrix(values);

    {
        s32 duration = KEY(g_CameraPathKey << 5).duration;
        s32 nextTick = g_CameraPathTick + 1;

        nextTick = (nextTick < duration) ? nextTick : 0;
        g_CameraPathTick = nextTick;
        if (nextTick == 0) {
            s32 next = g_CameraPathNextKey;
            s32 following = next + 1;

            g_CameraPathKey = next;
            g_CameraPathNextKey =
                (KEY(following << 5).duration == -1) ? 0 : following;
        }
    }
}

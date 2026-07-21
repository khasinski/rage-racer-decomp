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
        value = scaleBase + value;
        scaleBase = *(s16 *)(value + 4);
        asm("" : "=r"(scaleBase) : "0"(scaleBase));
        tone = (s16)tone;
        pitch = (s16)pitch;
        callType = 0x13;
        value = (result << 9) >> 16;
        result = (s16)func_80077C7C(callType, scaleBase, tone, pitch, 0x3C, 0, value, value);
        *handle = result;
    }

    D_80082F44 = savedCue;
    return result;
}

extern s32 D_80011F5C[][6];
extern s32 D_80011C84;
extern s16 D_801E6CA8[];

s32 func_8007B088(s32 arg0);

s32 func_8005D530(s32 arg0, s32 x, s32 y) {
    register s32 id asm("$20");
    register s32 pan asm("$18");
    register s32 tone asm("$19");
    register s32 pitch asm("$22");
    register s32 leftVol asm("$17");
    register s32 rightVol asm("$16");
    register s32 result asm("$21");
    s32 baseVol;
    s32 scale;
    s32 leftProduct;
    s32 rightProduct;

    id = arg0;
    baseVol = D_80011F5C[id][0];
    pan = D_80011F5C[id][1];
    tone = D_80011F5C[id][2];
    pitch = D_80011F5C[id][3];

    leftProduct = baseVol * x;
    if (leftProduct < 0) {
        leftProduct += 0x7F;
    }
    scale = D_801E6CA4;
    leftVol = (leftProduct >> 7) * scale;
    if (leftVol < 0) {
        leftVol += 0x7F;
    }
    rightProduct = baseVol * y;
    if (rightProduct < 0) {
        rightProduct += 0x7F;
    }
    leftVol = leftVol >> 7;
    rightVol = (rightProduct >> 7) * scale;
    if (rightVol < 0) {
        rightVol += 0x7F;
    }
    rightVol = rightVol >> 7;

    if (func_8007B088(D_80011C84) != 0 && id != 0x3D && id != 0x2B) {
        return result;
    }

    result = (s16)func_80077C7C(
        0x16, D_801E6CA8[pan], (s16)tone, (s16)pitch, 0x3C, 0, (s16)leftVol, (s16)rightVol);
    D_801E4D90 = result;
    result = (s16)func_80077C7C(
        0x17, D_801E6CA8[pan], (s16)tone, (s16)(pitch + 1), 0x3C, 0, (s16)leftVol, (s16)rightVol);
    D_801E4D94 = result;
    return result;
}

#include "common.h"

typedef struct SoundScale {
    s32 scale;
    s16 values[3];
} SoundScale;

extern s32 D_80082F44;
extern s32 D_801E4D90;
extern s32 D_801E4D94;
extern SoundScale D_801E6CA4;
extern s16 D_801E6CA8[];
extern s32 D_80011C84;
extern const s32 D_80011C8C[][6];
extern const s32 D_80011F5C[][6];

s32 func_80077C7C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
s32 func_8007B088(s32 arg0);

s32 func_8005D414(s32 cue, s32 volume) {
    s32 result = -1;
    s32 *handle;
    s32 value;
    s32 offset;
    s32 tone;
    s32 pitch;
    s32 scaled;
    s32 scaleValue;
    s32 current;

    handle = &D_801E4D90;
    D_801E4D94 = result;
    *handle = result;
    current = D_80082F44;

    if (current != cue) {
        scaled = volume * D_80011C8C[cue][0];
        offset = D_80011C8C[cue][1];
        tone = D_80011C8C[cue][2];
        pitch = D_80011C8C[cue][3];
        if (scaled < 0) {
            scaled += 0x7F;
        }

        result = D_801E6CA4.scale;
        value = scaled >> 7;
        value *= result;
        result = value;
        if (result < 0) {
            result += 0x7F;
        }

        scaleValue = D_801E6CA4.values[offset];
        tone = (s16)tone;
        pitch = (s16)pitch;
        result = (s16)func_80077C7C(
            0x13,
            scaleValue,
            tone,
            pitch,
            0x3C,
            0,
            (s32)((u32)result << 9) >> 16,
            (s32)((u32)result << 9) >> 16);
        *handle = result;
    }

    D_80082F44 = cue;
    return result;
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005D414", func_8005D530);

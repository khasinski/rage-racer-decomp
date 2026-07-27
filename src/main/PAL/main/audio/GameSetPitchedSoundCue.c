#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"
#include "game/sound.h"

extern s32 g_EffectVoiceState[] asm("D_801E6D38");
extern u8 g_EffectVoiceTone[] asm("D_801E6D34");
extern u8 g_EffectVoiceVolume[] asm("D_801E6D40");

long SsUtKeyOffV(long voice) asm("func_80078018");
void func_80078528(s32, s16, s16);
void func_800781C0(s32, s32, s32, s32, s32, s32, s32);

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/audio/GameSetPitchedSoundCue", func_8005C914);

/* Loop over the 4 effect voices (indices 10..13). `voice` is kept in the
 * compiler's scaled (<<16) representation of the short voice number, exactly
 * as GCC materialises it, and read back with `voice >> 16`. */
#define VOLPITCH()                                                    \
    svArg = voiceCopy;                                                \
    asm("" : "=r"(svArg) : "0"(svArg));                               \
    prod = *(s32 *)(g_EffectVoiceVolume + offset) * g_EffectVolumeScale;                \
    left = prod;                                                      \
    if (prod < 0) {                                                   \
        left = prod + 0x7F;                                           \
    }                                                                 \
    left >>= 7;                                                       \
    right = left;                                                     \
    if (right >= 0) {                                                 \
        if (right >= 0x81) {                                          \
            left = 0x80;                                              \
        }                                                             \
    } else {                                                          \
        left = 0;                                                     \
    }                                                                 \
    if (right >= 0) {                                                 \
        if (right >= 0x81) {                                          \
            right = 0x80;                                             \
        }                                                             \
    } else {                                                          \
        right = 0;                                                    \
    }                                                                 \
    func_80078528((s16)svArg, left, right);                           \
    func_800781C0(voice >> 16, 0, *f0Ptr, 0x3C, 0,                    \
                  (s16)(*pitchPtr >> 7), *(u16 *)pitchPtr & 0x7F);    \
    *statePtr = neg

void func_8005CDB0(void) {
    register s32 *statePtr asm("$16");
    register s32 *pitchPtr asm("$17");
    register s16 *f0Ptr asm("$20");
    register s32 offset asm("$19");
    register s32 voiceCopy asm("$21");
    register s32 neg asm("$22");
    register s32 svArg asm("$4");
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 prod asm("$2");
    s32 voice;
    s32 state;

    neg = -1;
    statePtr = g_EffectVoiceState;
    voice = 10 << 16;
    voiceCopy = 10;
    pitchPtr = statePtr + 1;
    f0Ptr = (s16 *)statePtr - 4;
    offset = 0;
    do {
        state = *statePtr;
        switch (state) {
        case 0:
            SsUtKeyOnV(voice >> 16, g_VabIds[0], *f0Ptr,
                          *(s16 *)(g_EffectVoiceTone + offset), 0x3C, 0, 0, 0);
            VOLPITCH();
            break;
        case 2:
            VOLPITCH();
            break;
        case 1:
            SsUtKeyOffV(voice >> 16);
            *statePtr = neg;
            break;
        }
        statePtr = (s32 *)((u8 *)statePtr + 0x14);
        voice += 1 << 16;
        voiceCopy++;
        pitchPtr = (s32 *)((u8 *)pitchPtr + 0x14);
        f0Ptr = (s16 *)((u8 *)f0Ptr + 0x14);
        offset += 0x14;
    } while ((s32)statePtr < (s32)&g_EffectVoiceState[20]);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/audio/GameSetPitchedSoundCue", func_8005D050);

extern s32 g_ActiveSpecialCue asm("D_80082F44");
extern s32 g_SpecialCueVoiceA asm("D_801E4D90");
extern s32 g_SpecialCueVoiceB asm("D_801E4D94");
/* Struct view of the same two objects audio.h names g_EffectVolumeScale
 * (D_801E6CA4) and g_VabIds (D_801E6CA8) -- not a third global. Spelling the
 * three reads below as those two scalars instead compiles but does not match:
 * gcc 2.6.3 treats a struct member reference as non-aliasing (MEM_IN_STRUCT_P)
 * and reorders the surrounding volume arithmetic. */
extern SoundScale g_SoundScale asm("D_801E6CA4");
extern s32 D_80011C84;
extern const s32 g_SoundCueParams[][6] asm("D_80011C8C");
extern const s32 g_SoundCueParams2[][6] asm("D_80011F5C");

s32 func_8007B088(s32 arg0);

s32 GameStartSingleSpecialCue(s32 cue, s32 volume) asm("func_8005D414");
s32 GameStartSingleSpecialCue(s32 cue, s32 volume) {
    s32 result = -1;
    s32 *handle;
    s32 value;
    s32 offset;
    s32 tone;
    s32 pitch;
    s32 scaled;
    s32 scaleValue;
    s32 current;

    handle = &g_SpecialCueVoiceA;
    g_SpecialCueVoiceB = result;
    *handle = result;
    current = g_ActiveSpecialCue;

    if (current != cue) {
        scaled = volume * g_SoundCueParams[cue][0];
        offset = g_SoundCueParams[cue][1];
        tone = g_SoundCueParams[cue][2];
        pitch = g_SoundCueParams[cue][3];
        if (scaled < 0) {
            scaled += 0x7F;
        }

        result = g_SoundScale.scale;
        value = scaled >> 7;
        value *= result;
        result = value;
        if (result < 0) {
            result += 0x7F;
        }

        scaleValue = g_SoundScale.values[offset];
        tone = (s16)tone;
        pitch = (s16)pitch;
        result = (s16)SsUtKeyOnV(
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

    g_ActiveSpecialCue = cue;
    return result;
}

s32 func_8005D530(s32 cue, s32 volumeLeft, s32 volumeRight) {
    s32 id;
    s32 pan;
    s32 prog;
    s32 tone;
    s32 sx;
    s32 sy;
    s32 result;
    s32 baseVol;
    s32 scale;
    s32 vx;
    s32 vy;
    s32 nextTone;

    id = cue;
    sy = volumeRight;
    baseVol = g_SoundCueParams2[id][0];
    pan = g_SoundCueParams2[id][1];
    prog = g_SoundCueParams2[id][2];
    tone = g_SoundCueParams2[id][3];

    vx = baseVol * volumeLeft;
    if (vx < 0) {
        vx += 0x7F;
    }
    scale = g_SoundScale.scale;
    sx = (vx >> 7) * scale;
    if (sx < 0) {
        sx += 0x7F;
    }
    vy = baseVol * sy;
    if (vy < 0) {
        vy += 0x7F;
    }
    sx >>= 7;
    volumeRight = (vy >> 7) * scale;
    if (volumeRight < 0) {
        volumeRight += 0x7F;
    }
    sy = volumeRight >> 7;

    if ((func_8007B088(D_80011C84) == 0) || (id == 0x3D) || (id == 0x2B)) {
        result = (s16)SsUtKeyOnV(
            0x16,
            g_VabIds[pan],
            (s16)prog,
            (s16)tone,
            0x3C,
            0,
            (s16)sx,
            (s16)sy);
        nextTone = tone + 1;
        nextTone = (s32)((u32)nextTone << 16) >> 16;
        result = (s16)SsUtKeyOnV(
            0x17,
            g_VabIds[(g_SpecialCueVoiceA = result, pan)],
            (s16)prog,
            nextTone,
            0x3C,
            0,
            (s16)sx,
            (s16)sy);
        g_SpecialCueVoiceB = result;
    }

    return result;
}

extern s32 g_SoundCueBank asm("D_801E6CA0");
extern s32 g_LastSpecialCueRequest asm("D_80082F48");

void func_8005D050(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void GamePlaySoundCue(s32 arg0) {
    s32 test;

    if (g_SoundCueBank == 1) {
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

    if (g_SoundCueBank == 2) {
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
            if (arg0 != g_LastSpecialCueRequest) {
                g_LastSpecialCueRequest = arg0;
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

extern s16 g_SoundSlotTone[][2] asm("D_80082F28");

void func_80078528(s32 voice, s16 left, s16 right);
void func_80078130(s32 voice, s32 vab_id, s32 program, s32 tone, s16 bend);

void func_8005D7D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) {
    register s32 voice asm("$4");
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 prod asm("$2");
    s32 bend;
    s32 voiceCopy;

    prod = arg2 * g_EffectVolumeScale;
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
    func_80078130((s16)voice, g_VabIds[(s16)bend], g_SoundSlotTone[arg0][arg3], 0x3C, arg1);
}

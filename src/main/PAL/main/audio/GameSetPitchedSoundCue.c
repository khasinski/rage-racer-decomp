#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"
#include "game/sound.h"

extern s32 g_EffectVoiceState[] asm("D_801E6D38");
extern u8 g_EffectVoiceTone[] asm("D_801E6D34");
extern u8 g_EffectVoiceVolume[] asm("D_801E6D40");
extern s32 g_EffectVoicePitch[] asm("D_801E6D3C");

extern const s32 g_EffectCueTable[] asm("D_80012730");
extern s32 D_801E6D30;
extern s32 D_801E6D44;
extern s32 D_801E6D58;
extern s32 D_801E6D60;
extern s32 D_801E6D6C;
extern const s32 D_80012734;
extern const s32 D_80012738;
extern const s32 D_80012740;
extern const s32 D_80012750;
extern const s32 D_80012758;
extern const s32 D_80012768;
extern const s32 D_80012770;

typedef struct EffectCueRow {
    s32 count;
    s32 scale;
    s32 cue;
    s32 tone;
} EffectCueRow;

long SsUtKeyOffV(long voice) asm("func_80078018");
void func_80078528(s32, s16, s16);
void func_800781C0(s32, s32, s32, s32, s32, s32, s32);

void GameSetPitchedSoundCue(s32 bank, s32 pitch, s32 volume) {
    s32 count;
    s32 loopCount;
    s32 i;
    s32 off;
    /* Load-bearing GCC 2.6.3 register roles for the table loops. */
    register s32 tblOff asm("$3");
    register s32 loopTblOff asm("$8");
    s32 compareOff;
    s32 scaled;
    s32 scaleValue;
    s32 cueValue;
    s32 toneValue;
    s32 hasActiveVoice;
    s32 ok;
    s32 bankIndex;
    s32 active;
    s32 inactive;
    s32 defaultPitch;
    register const EffectCueRow *p asm("$7");
    const s32 *tableBase;
    s32 *stateBase;

    if (bank >= 0) {
        if (bank >= 3) {
            bank = 2;
        }
    } else {
        bank = 0;
    }
    if (volume >= 0) {
        if (volume >= 0x80) {
            volume = 0x7F;
        }
    } else {
        volume = 0;
    }

    switch (bank) {
    case 0:
        if (volume <= 0) {
            s32 resetIndex;

            resetIndex = 0;
            if ((D_801E6D30 >= 0) || (D_801E6D44 >= 0)) {
                count = g_EffectCueTable[0];
                if (count > 0) {
                    active = 1;
                    inactive = -1;
                    defaultPitch = 0x1E00;
                    volume = count;
                    off = 0;
                    do {
                        *(s32 *)((u8 *)g_EffectVoiceState + off) = active;
                        *(s32 *)((u8 *)&D_801E6D30 + off) = inactive;
                        *(s32 *)((u8 *)g_EffectVoiceTone + off) = inactive;
                        *(s32 *)((u8 *)g_EffectVoicePitch + off) = defaultPitch;
                        *(s32 *)((u8 *)g_EffectVoiceVolume + off) = 0;
                        /* Keep the two loop updates in the retail schedule. */
                        asm("");
                        resetIndex++;
                        off += 0x14;
                    } while (resetIndex < volume);
                }
            }
        } else {
            if ((D_801E6D30 == D_80012738) &&
                (D_801E6D44 == D_80012740)) {
                *g_EffectVoiceState = 2;
            } else {
                *g_EffectVoiceState = 0;
            }
            bankIndex = (bank << 1) + bank;
            tblOff = bankIndex << 3;
            count = *(s32 *)((u8 *)g_EffectCueTable + tblOff);
            i = 0;
            if (count > i) {
                stateBase = g_EffectVoiceState;
                loopCount = count;
                tableBase = g_EffectCueTable;
                loopTblOff = tblOff;
                p = (const EffectCueRow *)((u8 *)tableBase + loopTblOff);
                off = 0;
                do {
                    if (i != 0) {
                        *(s32 *)((u8 *)g_EffectVoiceState + off) = *stateBase;
                    }
                    scaleValue = *(s32 *)((u8 *)&D_80012734 + loopTblOff);
                    scaled = volume * scaleValue;
                    cueValue = p->cue;
                    *(s32 *)((u8 *)&D_801E6D30 + off) = cueValue;
                    toneValue = p->tone;
                    *(s32 *)((u8 *)g_EffectVoicePitch + off) = pitch;
                    *(s32 *)((u8 *)g_EffectVoiceTone + off) = toneValue;
                    p = (const EffectCueRow *)((u8 *)p + 8);
                    if (scaled < 0) {
                        scaled += 0x7F;
                    }
                    *(s32 *)((u8 *)g_EffectVoiceVolume + off) = scaled >> 7;
                    i++;
                    off += 0x14;
                } while (i < loopCount);
            }
        }
        break;

    case 1:
    case 2:
        if (volume <= 0) {
            hasActiveVoice = D_801E6D58 >= 0;
            ok = 0;
            if (hasActiveVoice || (D_801E6D6C >= 0)) {
                if (bank == 1) {
                    if (D_801E6D58 == D_80012750) {
                        ok = D_801E6D6C == D_80012758;
                    }
                } else if ((bank == 2) && (D_801E6D58 == D_80012768) &&
                           (D_801E6D6C == D_80012770)) {
                    ok = 1;
                }
                if (ok != 0) {
                    count = g_EffectCueTable[bank * 6];
                    i = 0;
                    if (count > i) {
                        active = 1;
                        inactive = -1;
                        defaultPitch = 0x1E00;
                        volume = count;
                        off = 0x28;
                        do {
                            *(s32 *)((u8 *)g_EffectVoiceState + off) = active;
                            *(s32 *)((u8 *)&D_801E6D30 + off) = inactive;
                            *(s32 *)((u8 *)g_EffectVoiceTone + off) = inactive;
                            *(s32 *)((u8 *)g_EffectVoicePitch + off) = defaultPitch;
                            *(s32 *)((u8 *)g_EffectVoiceVolume + off) = 0;
                            /* Keep the two loop updates in the retail schedule. */
                            asm("");
                            i++;
                            off += 0x14;
                        } while (i < volume);
                    }
                }
            }
        } else {
            compareOff = bank * 0x18;
            if ((D_801E6D58 ==
                 *(s32 *)((u8 *)&D_80012738 + compareOff)) &&
                (D_801E6D6C ==
                 *(s32 *)((u8 *)&D_80012740 + compareOff))) {
                D_801E6D60 = 2;
                tblOff = bank * 2;
            } else {
                D_801E6D60 = 0;
                tblOff = bank * 2;
            }
            tblOff = (tblOff + bank) * 8;
            bankIndex = (bank << 1) + bank;
            tblOff = bankIndex << 3;
            count = *(s32 *)((u8 *)g_EffectCueTable + tblOff);
            i = 0;
            if (count > i) {
                stateBase = &D_801E6D60;
                loopCount = count;
                tableBase = g_EffectCueTable;
                loopTblOff = tblOff;
                p = (const EffectCueRow *)((u8 *)tableBase + loopTblOff);
                off = 0x28;
                do {
                    if (i != 0) {
                        *(s32 *)((u8 *)g_EffectVoiceState + off) = *stateBase;
                    }
                    scaleValue = *(s32 *)((u8 *)&D_80012734 + loopTblOff);
                    scaled = volume * scaleValue;
                    cueValue = p->cue;
                    *(s32 *)((u8 *)&D_801E6D30 + off) = cueValue;
                    toneValue = p->tone;
                    *(s32 *)((u8 *)g_EffectVoicePitch + off) = pitch;
                    *(s32 *)((u8 *)g_EffectVoiceTone + off) = toneValue;
                    p = (const EffectCueRow *)((u8 *)p + 8);
                    if (scaled < 0) {
                        scaled += 0x7F;
                    }
                    *(s32 *)((u8 *)g_EffectVoiceVolume + off) = scaled >> 7;
                    i++;
                    off += 0x14;
                } while (i < loopCount);
            }
        }
        break;
    }
}

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
    s32 *statePtr;
    s32 *pitchPtr;
    s16 *f0Ptr;
    s32 offset;
    s32 voiceCopy;
    s32 neg;
    s32 svArg;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 left asm("$5");
    s32 right;
    s32 prod;
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

extern s32 g_SoundCueBank asm("D_801E6CA0");
extern SoundScale g_SoundScale asm("D_801E6CA4");
extern s32 g_SpecialCueVoiceA asm("D_801E4D90");
extern s32 g_SpecialCueVoiceB asm("D_801E4D94");
extern const s32 g_SoundCueParams[][6] asm("D_80011C8C");
extern const s32 g_SoundCueParams2[][6] asm("D_80011F5C");
extern const s32 g_SpecialVoiceBits[] asm("D_80011C74");
extern const char D_80012778[];
s32 SpuGetKeyStatus(s32 bit) asm("func_8007B088");

s32 func_8005D050(s32 cue, s32 arg1, s32 volL, s32 volR) {
    const s32 *voiceBits;
    s32 busy[6];
    s32 tone2;
    s32 vab;
    s32 prog;
    s32 tone;
    s32 baseVol;
    s32 scale;
    s32 scaled;
    s32 result;
    s32 i;

    tone = 0;
    voiceBits = g_SpecialVoiceBits;
    tone2 = 1;
    if (g_SoundCueBank == 1) {
        tone2 = g_SoundCueParams[cue][4];
        vab = g_SoundCueParams[cue][1];
        prog = g_SoundCueParams[cue][2];
        tone = g_SoundCueParams[cue][3];
        baseVol = g_SoundCueParams[cue][0];
    } else {
        vab = 0;
        if (g_SoundCueBank == 2) {
            tone2 = g_SoundCueParams2[cue][4];
            vab = g_SoundCueParams2[cue][1];
            prog = g_SoundCueParams2[cue][2];
            tone = g_SoundCueParams2[cue][3];
            baseVol = g_SoundCueParams2[cue][0];
        } else {
            prog = cue;
            baseVol = 0x80;
        }
    }

    scaled = volL * baseVol;
    if (scaled < 0) {
        scaled += 0x7F;
    }
    scale = g_SoundScale.scale;
    scaled = (scaled >> 7) * scale;
    if (scaled < 0) {
        scaled += 0x7F;
    }
    volL = scaled >> 7;
    scaled = volR * baseVol;
    if (scaled < 0) {
        scaled += 0x7F;
    }
    scaled = (scaled >> 7) * scale;
    if (scaled < 0) {
        scaled += 0x7F;
    }
    volR = scaled >> 7;

    if (g_SoundCueBank == 1) {
        i = 0;
        do {
            busy[i] = SpuGetKeyStatus(voiceBits[i]);
            i++;
        } while (i < 6);
        i = 0;
        do {
            if (busy[i] == 0) {
                result = (s16)SsUtKeyOnV((s16)(i + 0x12), g_SoundScale.values[vab],
                                         (s16)prog, (s16)tone, 0x3C, 0,
                                         (s16)volL, (s16)volR);
                g_SpecialCueVoiceA = result;
                busy[i] = 1;
                break;
            }
            i++;
        } while (i < 6);
        i = 0;
        do {
            if (busy[i] == 0) {
                result = (s16)SsUtKeyOnV((s16)(i + 0x12), g_SoundScale.values[vab],
                                         (s16)prog, (s16)tone2, 0x3C, 0,
                                         (s16)volL, (s16)volR);
                g_SpecialCueVoiceB = result;
                busy[i] = 1;
                break;
            }
            i++;
        } while (i < 6);
    } else {
        result = (s16)SsUtKeyOn(g_SoundScale.values[vab], prog, tone, 0x3C, 0,
                                volL, volR);
        result = (s16)SsUtKeyOn(g_SoundScale.values[(g_SpecialCueVoiceA = result, vab)],
                                prog, tone2, 0x3C, 0, volL, volR);
        g_SpecialCueVoiceB = result;
    }

    if (result < 0) {
        GameDebugPrintf(D_80012778);
        return -1;
    }
    return result;
}

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

s32 SpuGetKeyStatus(s32 arg0) asm("func_8007B088");

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

    if ((SpuGetKeyStatus(D_80011C84) == 0) || (id == 0x3D) || (id == 0x2B)) {
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

s32 func_8005D050(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

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
    s32 voice;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 left asm("$5");
    s32 right;
    s32 prod;
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

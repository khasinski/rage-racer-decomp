#include "common.h"
#include "game/audio.h"
#include "game/sound.h"
#include "psyq/snd.h"

extern s32 g_IndexedEffectIndexPrev asm("D_801E6CF4");
extern s32 g_IndexedEffectPitch asm("D_801E6CF8");
extern s32 g_IndexedEffectVolume asm("D_801E6CFC");

long SsUtKeyOffV(long voice) asm("func_80078018");
void func_8005C09C(s32 arg0);
void func_8005C0E4(void);

void GameForceBasicEffectVoicesEnabled(s32 enabled) asm("func_8005DEF0");
void GameForceBasicEffectVoicesEnabled(s32 enabled) {
    s32 offset;
    s32 voicePacked;
    s32 voice;
    s32 i;
    s32 unused;
    s32 raw;
    s32 scale;
    s32 left;
    s32 right;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 arg0 asm("$4");
    s32 zeroArg;

    unused = 0;
    i = 0;
    voicePacked = 0x80000;
    voice = 8;
    offset = 0;
    do {
        if (enabled != 0) {
            arg0 = voicePacked >> 16;
            raw = 0x3C;
            left = g_VabIds[0];
            right = *(s16 *)((u8 *)&g_MusicChannels[0].left + offset);
            zeroArg = 0;
            SsUtKeyOnV(arg0, left, right, zeroArg, raw, 0, 0, 0);
            asm volatile("" : : "r"(unused));

            raw = *(s32 *)((u8 *)&g_MusicChannels[0].volLeft + offset);
            scale = g_EffectVolumeScale;
            left = raw * scale;
            raw = *(s32 *)((u8 *)&g_MusicChannels[0].volRight + offset);
            arg0 = voice;
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

            SsUtSetVVol((s16)arg0, left, right);
        } else {
            SsUtKeyOffV(voicePacked >> 16);
        }

        voicePacked += 0x10000;
        voice++;
        i++;
        offset += 0x18;
    } while (i < 2);
}

void GameForceIndexedEffectVoiceEnabled(s32 enabled) asm("func_8005E058");
void GameForceIndexedEffectVoiceEnabled(s32 enabled) {
    s32 base;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 center;
    s32 fine;
    register s32 index asm("$3");
    register s32 raw asm("$2");
    s32 product;
    s32 scale;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 voice asm("$4");

    if (enabled != 0) {
        index = g_IndexedEffectIndexPrev;
        if (index < 0) {
            return;
        }
        raw = (index * 3) << 2;
        func_8005C09C(INDEXED_EFFECT(raw).tone);
    } else {
        func_8005C0E4();
    }

    raw = g_IndexedEffectIndexPrev;
    if (raw >= 0) {
        index = (raw * 3) << 2;
        product = g_IndexedEffectVolume * INDEXED_EFFECT(index).volume;
        raw = g_IndexedEffectPitch;
        base = INDEXED_EFFECT(index).tone;
        center = raw >> 7;
        fine = raw & 0x7F;
        if (product < 0) {
            product += 0x7F;
        }
        raw = product >> 7;
        scale = g_EffectVolumeScale;
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
}

void GameForcePitchEffectVoicesEnabled(s32 enabled) asm("func_8005E1D0");
void GameForcePitchEffectVoicesEnabled(s32 enabled) {
    s32 voicePacked;
    s32 voice;
    s32 pitchBase;
    s32 toneBase;
    s32 offset;
    s32 state;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 raw asm("$2");
    s32 scale;
    s32 left;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 right asm("$6");
    register s32 arg0 asm("$4");
    s32 arg3;

    state = enabled;
    voicePacked = 0xA0000;
    voice = 0xA;
    pitchBase = (s32)&g_EffectVoices[0].pitch;
    toneBase = pitchBase - 0xC;
    offset = 0;
    do {
        if (state != 0) {
            arg0 = voicePacked >> 16;
            left = g_VabIds[0];
            right = *(s16 *)toneBase;
            arg3 = *(s16 *)((u8 *)&g_EffectVoices[0].tone + offset);
            raw = 0x3C;
            SsUtKeyOnV(arg0, left, right, arg3, raw, 0, 0, 0);

            scale = *(s32 *)((u8 *)&g_EffectVoices[0].volume + offset);
            asm volatile("" : : "r"(scale));
            raw = g_EffectVolumeScale;
            raw = scale * raw;
            arg0 = voice;
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

            SsUtSetVVol((s16)arg0, left, right);

            right = *(s16 *)toneBase;
            arg0 = voicePacked >> 16;
            SsUtChangePitch(arg0, 0, right, 0x3C, 0,
                            (*(s32 *)pitchBase << 9) >> 16,
                            *(u16 *)pitchBase & 0x7F);
        } else {
            SsUtKeyOffV(voicePacked >> 16);
        }

        voicePacked += 0x10000;
        voice++;
        pitchBase += 0x14;
        toneBase += 0x14;
        offset += 0x14;
    } while (pitchBase < (s32)&g_ReverbFadeStep);
}

/* Mid-function labels the retail build exports (see docs/ASM_AND_GTE_POLICY.md,
 * "symbol labels"). They define symbols only and emit no code. */
asm(".globl func_8005E078\n"
    "func_8005E078 = GameForceIndexedEffectVoiceEnabled + 0x20\n"
    ".globl func_8005E1B8\n"
    "func_8005E1B8 = GameForceIndexedEffectVoiceEnabled + 0x160\n"
    ".globl func_8005E200\n"
    "func_8005E200 = GameForcePitchEffectVoicesEnabled + 0x30\n"
    ".globl func_8005E314\n"
    "func_8005E314 = GameForcePitchEffectVoicesEnabled + 0x144");

extern s16 g_SoundSlotTone[] asm("D_80082F28");
extern s16 g_SoundSlotToneBank1[] asm("D_80082F2A");
extern s32 g_SoundSlotActive[] asm("D_801E6CC8");

void GameSetSoundSlotVoicesEnabledWithRegisterArg(void) asm("func_8005B40C");
void GamePlaySoundSlotVoice(s32 slot, s32 tone, s32 vabSlot) asm("func_8005B2F0");
void func_8005D7D4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void GameForceSoundSlotVoicePlayback(s32 arg0) {
    s32 saved = arg0;
    s32 i;
    /* This pin is load-bearing: removing it changes .text. */
    register s32 *base asm("$18");
    s32 *active;
    s32 odd;
    s32 first;
    s32 second;
    s32 factor;
    s32 scaled;
    s32 call_arg0;
    s32 call_arg1;
    s32 call_arg3;

    GameSetSoundSlotVoicesEnabledWithRegisterArg();

    i = 0;
    if (saved != 0) {
        base = g_SoundSlotActive;
        active = base;
        saved = 0;
        do {
            if (*base++ != 0 && *(s16 *)((s32)g_SoundSlotTone + saved) != *(s16 *)((s32)g_SoundSlotToneBank1 + saved)) {
                GamePlaySoundSlotVoice(i, active[-3], 3);
            }
            i++;
            saved += 4;
        } while (i < 6);

        i = 0;
        odd = 1;
        base = g_SoundSlotActive;
        active = base;
        do {
            if (*active != 0) {
                first = GameInterpolateAudioParameter(i * 2, base[-4], base[-3]);
                second = GameInterpolateAudioParameter(odd, base[-4], base[-3]);
                factor = base[6];
                scaled = second * factor;
                if (scaled < 0) {
                    scaled += 0x7F;
                }
                call_arg0 = i;
                call_arg1 = first;
                scaled >>= 7;
                call_arg3 = base[-3];
                func_8005D7D4(call_arg0, call_arg1, scaled, call_arg3, 3);
            }
            odd += 2;
            i++;
            active++;
        } while (i < 6);
    }
}

void GameForceAllEffectVoicesEnabled(s32 arg0) {
    GameForcePanVoiceEnabled(arg0);
    GameForceBasicEffectVoicesEnabled(arg0);
    GameForceIndexedEffectVoiceEnabled(arg0);
    GameForcePitchEffectVoicesEnabled(arg0);
    GameForceSoundSlotVoicePlayback(arg0);
}

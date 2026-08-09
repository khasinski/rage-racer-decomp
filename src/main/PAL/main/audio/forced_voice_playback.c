#include "common.h"
#include "game/audio.h"
#include "game/sound.h"
#include "psyq/snd.h"

void ForceBasicEffectVoicesEnabled(s32 enabled) {
    s32 voicePacked;
    s32 voice;
    s32 i;
    s32 unused;
    s32 raw;
    s32 scale;
    s32 left;
    s32 right;
    register s32 voiceArg asm("$4");
    s32 zeroArg;

    unused = 0;
    i = 0;
    voicePacked = 0x80000;
    voice = 8;
    do {
        if (enabled != 0) {
            voiceArg = voicePacked >> 16;
            raw = 0x3C;
            left = g_VabIds[0];
            right = g_MusicChannels[i].left.half[0];
            zeroArg = 0;
            SsUtKeyOnV(voiceArg, left, right, zeroArg, raw, 0, 0, 0);
            asm volatile("" : : "r"(unused));

            raw = g_MusicChannels[i].volLeft;
            scale = g_SoundScale.scale;
            left = raw * scale;
            raw = g_MusicChannels[i].volRight;
            voiceArg = voice;
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

            SsUtSetVVol((s16)voiceArg, left, right);
        } else {
            SsUtKeyOffV(voicePacked >> 16);
        }

        voicePacked += 0x10000;
        voice++;
        i++;
    } while (i < 2);
}

void ForceIndexedEffectVoiceEnabled(s32 enabled) {
    s32 base;
    register s32 center;
    s32 fine;
    register s32 index asm("$3");
    register s32 raw asm("$2");
    s32 product;
    s32 scale;
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 voice asm("$4");

    if (enabled != 0) {
        index = g_IndexedEffectIndexPrev;
        if (index < 0) {
            return;
        }
        raw = (index * 3) << 2;
        StartIndexedEffectVoice(INDEXED_EFFECT(raw).tone);
    } else {
        StopIndexedEffectVoice();
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
        scale = g_SoundScale.scale;
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

void ForcePitchEffectVoicesEnabled(s32 enabled) {
    s32 voicePacked;
    s32 voice;
    s32 pitchBase;
    s32 toneBase;
    s32 offset;
    s32 state;
    register s32 raw asm("$2");
    s32 scale;
    s32 left;
    register s32 right asm("$6");
    register s32 voiceArg asm("$4");
    s32 keyTone;

    state = enabled;
    voicePacked = 0xA0000;
    voice = 0xA;
    pitchBase = (s32)&g_EffectVoices[0].pitch;
    toneBase = pitchBase - 0xC;
    offset = 0;
    do {
        if (state != 0) {
            voiceArg = voicePacked >> 16;
            left = g_VabIds[0];
            right = *(s16 *)toneBase;
            keyTone =
                (s16)((EffectVoice *)((u8 *)g_EffectVoices + offset))->tone;
            raw = 0x3C;
            SsUtKeyOnV(voiceArg, left, right, keyTone, raw, 0, 0, 0);

            scale = ((EffectVoice *)((u8 *)g_EffectVoices + offset))->volume;
            asm volatile("" : : "r"(scale));
            raw = g_SoundScale.scale;
            raw = scale * raw;
            voiceArg = voice;
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

            SsUtSetVVol((s16)voiceArg, left, right);

            right = *(s16 *)toneBase;
            voiceArg = voicePacked >> 16;
            SsUtChangePitch(voiceArg, 0, right, 0x3C, 0,
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

void ForceSoundSlotVoicePlayback(s32 enabled) {
    s32 saved = enabled;
    s32 i;
    register s32 *base asm("$18");
    s32 *active;
    s32 odd;
    s32 first;
    s32 second;
    s32 factor;
    s32 scaled;
    s32 callSlot;
    s32 callBend;
    s32 callTone;

    SetSoundSlotVoicesEnabled(enabled);

    i = 0;
    if (saved != 0) {
        base = g_SoundSlotActive;
        active = base;
        saved = 0;
        do {
            if (*base++ != 0 && g_SoundSlotTone[i][0] != g_SoundSlotTone[i][1]) {
                PlaySoundSlotVoice(i, active[-3], 3);
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
                first = InterpolateAudioParameter(i * 2, base[-4], base[-3]);
                second = InterpolateAudioParameter(odd, base[-4], base[-3]);
                factor = base[6];
                scaled = second * factor;
                if (scaled < 0) {
                    scaled += 0x7F;
                }
                callSlot = i;
                callBend = first;
                scaled >>= 7;
                callTone = base[-3];
                SetSoundSlotTone(callSlot, callBend, scaled, callTone, 3);
            }
            odd += 2;
            i++;
            active++;
        } while (i < 6);
    }
}

void ForceAllEffectVoicesEnabled(s32 enabled) {
    ForcePanVoiceEnabled(enabled);
    ForceBasicEffectVoicesEnabled(enabled);
    ForceIndexedEffectVoiceEnabled(enabled);
    ForcePitchEffectVoicesEnabled(enabled);
    ForceSoundSlotVoicePlayback(enabled);
}

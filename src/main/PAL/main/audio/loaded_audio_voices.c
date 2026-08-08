#include "common.h"
#include "game/audio.h"
#include "game/sound.h"
#include "psyq/snd.h"
#include "game/car.h"

s32 InterpolateAudioParameter(s32 parameter, s32 position, s32 bank) {
    s32 value = position;
    s32 index;
    s32 index_offset;
    s32 *base = (s32 *)g_EngineSoundCurves;
    s32 row_offset;
    s32 bank_offset;
    s32 *base_minus;
    s32 *entry;
    s32 *scan;
    s32 *lower_position;
    s32 lower_value_base;
    s32 lower_value_indexed;
    s32 lower_value_address;
    s32 upper_value_base;
    s32 upper_value_banked;
    s32 upper_value_address;
    s32 lower_position_value;
    s32 lower_value_value;
    s32 upper_value_value;
    s32 numerator;
    s32 denominator;
    s32 raw_result;
    s32 result;

    index = 1;
    row_offset = (parameter * 9) << 3;
    bank_offset = (((bank * 7) * 4) - bank) << 5;
    bank = row_offset + bank_offset;
    entry = (s32 *)((s32)base + bank);
    scan = entry + 1;
    while (index < 9) {
        raw_result = *scan;
        if (value < raw_result) {
            break;
        }
        index++;
        scan++;
    }

    base_minus = base - 1;
    index_offset = index * 4;
    lower_position = (s32 *)(index_offset + (s32)base_minus);
    lower_position = (s32 *)((u8 *)lower_position + bank);
    lower_value_base = (s32)(base + 8);
    lower_value_indexed = index_offset + lower_value_base;
    lower_value_address = lower_value_indexed + bank;
    upper_value_base = (s32)(base + 9);
    upper_value_banked = bank + upper_value_base;
    upper_value_address = index_offset + upper_value_banked;
    lower_value_value = *(s32 *)lower_value_address;
    upper_value_value = *(s32 *)upper_value_address;
    lower_position_value = *lower_position;
    numerator =
        (upper_value_value - lower_value_value) *
        (value - lower_position_value);
    denominator =
        *(s32 *)(index_offset + (s32)entry) - lower_position_value;
    raw_result = numerator / denominator + lower_value_value;

    if (raw_result >= 0) {
        result = raw_result;
        if (result >= 0x80) {
            result = 0x7F;
        }
    } else {
        result = 0;
    }

    return result;
}

void UpdateLoadedAudioVoices(s32 value, s32 bank) {
    s32 odd_parameter;
    s32 index;
    s32 second;
    s32 scaled;
    s32 *scale_base;
    s32 *slot;
    s32 *slot_base;
    s32 first;

    value = ((value * 5) << 11) / *(scale_base = &g_EngineSoundMaxRpm);

    if (bank != g_EngineSoundBank) {
        index = 0;
        slot = scale_base + 1;
        do {
            if (*slot++ != 0 &&
                g_SoundSlotTone[index][0] != g_SoundSlotTone[index][1]) {
                PlaySoundSlotVoice(index, bank, 3);
            }
            index++;
        } while (index < 6);
        g_EngineSoundBank = bank;
    }

    index = 0;
    odd_parameter = 1;
    scale_base = (slot_base = g_SoundSlotActive);
    slot = scale_base;
    do {
        if (*slot != 0) {
            first = InterpolateAudioParameter(index * 2, value, bank);
            second = InterpolateAudioParameter(odd_parameter, value, bank);
            scaled = second * slot_base[6];
            if (scaled < 0) {
                scaled += 0x7F;
            }
            SetSoundSlotTone(index, first, scaled >> 7, bank, 3);
        }
        odd_parameter += 2;
        index++;
        slot++;
    } while (index < 6);

    g_EngineSoundPosition = value;
    ApplyPanVoiceVolume();
    UpdateBasicEffectVoices();
    UpdateIndexedEffectVoice();
    UpdateEffectVoiceStates();
}

void SetDefaultReverbDepth(void) {
    SetReverbDepth(0x28, 0x28);
}

void InitSequenceAudio(void) {
    _SsVmInit(0);
    SsSetVoiceCount(0x12);
    SetReverbDepth(0x28, 0x28);
    g_ReverbFadeStep = 0;
    RefreshSequenceVolumeScale();
}

void InitEffectVoiceRuntime(void) {
    _SsVmInit(0);
    SsSetVoiceCount(8);

    {
        s32 i;
        s32 neg;
        register s32 *ptr asm("$5");
        s32 offset;

        i = 0;
        neg = -1;
        ptr = &g_AudioSlotMask;
        offset = 0;
        for (; i < 2; i++) {
            *(s32 *)((u8 *)&g_MusicChannels[0].mode + offset) = neg;
            *(s32 *)((u8 *)&g_MusicChannels[0].left + offset) = neg;
            *(s32 *)((u8 *)&g_MusicChannels[0].right + offset) = neg;
            ptr[0x78 / 4] = 0;
            ptr = (s32 *)((u8 *)ptr + 0x18);
            *(s32 *)((u8 *)&g_MusicChannels[0].volLeft + offset) = 0;
            offset += 0x18;
        }
    }

    {
        s32 i;
        s32 neg;
        s32 value;
        s32 offset;

        i = 0;
        neg = -1;
        value = 0x1E00;
        offset = 0;
        for (; i < 4; i++) {
            *(s32 *)((u8 *)&g_EffectVoices[0].state + offset) = neg;
            *(s32 *)((u8 *)&g_EffectVoices[0].note + offset) = neg;
            *(s32 *)((u8 *)&g_EffectVoices[0].tone + offset) = neg;
            *(s32 *)((u8 *)&g_EffectVoices[0].pitch + offset) = value;
            *(s32 *)((u8 *)&g_EffectVoices[0].volume + offset) = 0;
            offset += 0x14;
        }
    }

    {
        s32 value;

        value = -1;
        g_PanVoiceVolumeR = value;
        g_PanVoiceVolumeL = value;
        g_IndexedEffectIndexPrev = value;
        g_IndexedEffectIndex = value;
        value = 0x1E00;
        g_PanVoiceActive = 0;
        g_IndexedEffectPitch = value;
    }

    SetEffectVoicesEnabled(1);
    SetReverbPreset(2, 0, 0);
    SetLoadedTableVolumeScale(g_CarSoundVolumeScales[GetOwnedCarAssetIndex(g_PlayerCarIndex)]);
}

void RestoreReverbDepth(s32 enabled) {
    if (enabled != 0) {
        SetReverbDepth(g_ReverbDepthL, g_ReverbDepthR);
    } else {
        SetReverbDepth(0, 0);
    }
}

void ForcePanVoiceEnabled(s32 enabled) {
    s32 values[2];
    s32 i;
    s32 *src;
    s32 *dst;
    s32 scale;
    s32 raw;
    register s32 voice asm("$4");
    s32 left;
    register s32 right asm("$6");
    s32 zeroArg;
    register s32 unused asm("$16");

    i = 0;
    dst = values;
    src = &g_PanVoiceVolumeL;
    do {
        if (*src < 2) {
            *dst = 0;
        } else {
            *dst = *src;
        }
        dst++;
        i++;
        src++;
    } while (i < 2);

    if (enabled != 0) {
        raw = values[0];
        scale = g_SoundScale.scale;
        left = raw * scale;
        raw = values[1];
        if (left < 0) {
            left += 0x7F;
        }
        unused = 0;
        asm volatile("" : : "r"(unused));
        raw *= scale;
        left >>= 7;
        if (raw < 0) {
            raw += 0x7F;
        }
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

        SsUtSetVVol(0x15, left, right);
        voice = 0x15;
        right = 0xF;
        asm volatile("" : : "r"(voice), "r"(right));
        raw = 0x3C;
        left = g_VabIds[0];
        zeroArg = 0;
        SsUtKeyOnV(voice, left, right, zeroArg, raw, 0, 0, 0);
    } else {
        SsUtKeyOffV(0x15);
    }
}


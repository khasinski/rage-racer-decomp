#include "common.h"
#include "game/audio.h"
#include "game/sound.h"
#include "psyq/snd.h"
#include "game/car.h"

extern s16 g_SoundSlotTone[] asm("D_80082F28");
extern s16 g_SoundSlotToneBank1[] asm("D_80082F2A");
extern s32 g_EngineSoundPosition asm("D_801E6CB8");
extern s32 g_EngineSoundBank asm("D_801E6CBC");
extern s32 g_EngineSoundMaxRpm asm("D_801E6CC4");
extern s32 g_SoundSlotActive[] asm("D_801E6CC8");
extern s32 g_EngineSoundCurves[] asm("D_801E446C");

void func_8005B2F0(s32 slot, s32 tone, s32 vab_slot);
void func_8005D7D4(s32 slot, s32 left, s32 right, s32 bank, s32 mode);
void func_8005BF30(void);
void func_8005C6C0(void);
void func_8005C168(void);
void func_8005CDB0(void);
s32 GameInterpolateAudioParameter(s32 parameter, s32 position, s32 bank) asm("func_8005D8EC");
s32 GameInterpolateAudioParameter(s32 parameter, s32 position, s32 bank) {
    s32 value = position;
    s32 index;
    s32 index_offset;
    s32 *base = g_EngineSoundCurves;
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
    bank_offset = ((((bank * 8) - bank) * 4) - bank) << 5;
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
    index_offset = index << 2;
    lower_position = (s32 *)(index_offset + (s32)base_minus);
    lower_position = (s32 *)((s32)lower_position + bank);
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

void GameUpdateLoadedAudioVoices(s32 value, s32 bank) asm("func_8005D9F8");
void GameUpdateLoadedAudioVoices(s32 value, s32 bank) {
    s32 odd_parameter;
    s32 index;
    s32 tone_offset;
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
        tone_offset = 0;
        do {
            if (*slot++ != 0 &&
                *(s16 *)((s32)g_SoundSlotTone + tone_offset) !=
                    *(s16 *)((s32)g_SoundSlotToneBank1 + tone_offset)) {
                func_8005B2F0(index, bank, 3);
            }
            index++;
            tone_offset += 4;
        } while (index < 6);
        g_EngineSoundBank = bank;
    }

    index = 0;
    odd_parameter = 1;
    scale_base = (slot_base = g_SoundSlotActive);
    slot = scale_base;
    do {
        if (*slot != 0) {
            first = GameInterpolateAudioParameter(index * 2, value, bank);
            second = GameInterpolateAudioParameter(odd_parameter, value, bank);
            scaled = second * slot_base[6];
            if (scaled < 0) {
                scaled += 0x7F;
            }
            func_8005D7D4(index, first, scaled >> 7, bank, 3);
        }
        odd_parameter += 2;
        index++;
        slot++;
    } while (index < 6);

    g_EngineSoundPosition = value;
    func_8005BF30();
    func_8005C6C0();
    func_8005C168();
    func_8005CDB0();
}

void GameSetDefaultReverbDepth(void) {
    GameSetReverbDepth(0x28, 0x28);
}

extern s32 g_ReverbFadeStep asm("D_801E6D8C");
void func_8007865C(s32 arg0);
void func_80072B04(s32 arg0);
void func_8005B190(s32 arg0, s32 arg1);
void func_8005E7DC(void);
void GameInitSequenceAudio(void) asm("func_8005DBD8");
void GameInitSequenceAudio(void) {
    func_8007865C(0);
    func_80072B04(0x12);
    func_8005B190(0x28, 0x28);
    g_ReverbFadeStep = 0;
    func_8005E7DC();
}

extern s32 g_AudioSlotMask asm("D_801E6C9C");
extern s32 g_PanVoiceVolumeL asm("D_801E6CE4");
extern s32 g_PanVoiceVolumeR asm("D_801E6CE8");
extern s32 g_PanVoiceActive asm("D_801E6CEC");
extern s32 g_IndexedEffectIndex asm("D_801E6CF0");
extern s32 g_IndexedEffectIndexPrev asm("D_801E6CF4");
extern s32 g_IndexedEffectPitch asm("D_801E6CF8");
extern s32 g_CarSoundVolumeScales[] asm("D_800125FC");

s32 func_80050FA8(s32 arg0);

void GameInitEffectVoiceRuntime(void) asm("func_8005DC1C");
void GameInitEffectVoiceRuntime(void) {
    func_8007865C(0);
    SsSetVoiceCount(8);

    {
        register s32 i asm("$4");
        register s32 neg asm("$6");
        register s32 *ptr asm("$5");
        register s32 offset asm("$3");

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
        register s32 i asm("$4");
        register s32 neg asm("$5");
        register s32 value asm("$6");
        register s32 offset asm("$3");

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
        register s32 value asm("$2");

        value = -1;
        g_PanVoiceVolumeR = value;
        g_PanVoiceVolumeL = value;
        g_IndexedEffectIndexPrev = value;
        g_IndexedEffectIndex = value;
        value = 0x1E00;
        g_PanVoiceActive = 0;
        g_IndexedEffectPitch = value;
    }

    GameSetEffectVoicesEnabled(1);
    GameSetReverbPreset(2, 0, 0);
    GameSetLoadedTableVolumeScale(g_CarSoundVolumeScales[func_80050FA8(g_PlayerCarIndex)]);
}

extern s32 g_ReverbDepthL asm("D_801E6D84");
extern s32 g_ReverbDepthR asm("D_801E6D88");

void GameRestoreReverbDepth(s32 arg0) {
    if (arg0 != 0) {
        GameSetReverbDepth(g_ReverbDepthL, g_ReverbDepthR);
    } else {
        GameSetReverbDepth(0, 0);
    }
}

s32 SsUtKeyOffV(s32 voice) asm("func_80078018");

void GameForcePanVoiceEnabled(s32 enabled) asm("func_8005DDB8");
void GameForcePanVoiceEnabled(s32 enabled) {
    s32 values[2];
    register s32 i asm("$7");
    register s32 *src asm("$6");
    register s32 *dst asm("$5");
    register s32 scale asm("$3");
    register s32 raw asm("$2");
    register s32 voice asm("$4");
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 zeroArg asm("$7");
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
        scale = g_EffectVolumeScale;
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

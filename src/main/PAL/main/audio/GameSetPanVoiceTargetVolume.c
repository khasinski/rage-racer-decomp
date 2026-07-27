#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"
#include "game/sound.h"

extern s32 g_StereoOutput asm("D_80082F40");
extern s32 g_PanVoiceVolumeL asm("D_801E6CE4");
extern s32 g_PanVoiceVolumeR asm("D_801E6CE8");

void GameSetPanVoiceTargetVolume(s32 arg0, s32 arg1) {
    if (arg0 >= 0) {
        if (arg0 < 0x81) {
            goto arg0_done;
        }
        arg0 = 0x80;
    } else {
        arg0 = 0;
    }

arg0_done:
    if (arg1 >= 0) {
        if (arg1 < 0x81) {
            goto arg1_done;
        }
        arg1 = 0x80;
    } else {
        arg1 = 0;
    }

arg1_done:
    if (g_StereoOutput != 0) {
        g_PanVoiceVolumeL = arg0;
        g_PanVoiceVolumeR = arg1;
    } else {
        s32 temp = (arg0 + arg1) / 2;

        g_PanVoiceVolumeL = temp;
        g_PanVoiceVolumeR = temp;
    }
}

extern s32 g_PanVoiceActive asm("D_801E6CEC");

s32 SsUtKeyOffV(s32 voice) asm("func_80078018");

void GameApplyPanVoiceVolume(void) asm("func_8005BF30");
void GameApplyPanVoiceVolume(void) {
    s32 values[2];
    register s32 changed asm("$16");
    register s32 i asm("$6");
    register s32 *dst asm("$4");
    register s32 *src asm("$5");
    register s32 raw asm("$2");
    register s32 loopValue asm("$3");
    register s32 scale asm("$3");
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 voice asm("$4");
    register s32 zeroArg asm("$7");

    changed = 0;
    i = 0;
    dst = values;
    src = &g_PanVoiceVolumeL;
    do {
        loopValue = *src;
        if (loopValue < 2) {
            *dst = 0;
        } else {
            *dst = loopValue;
            changed = 1;
        }
        dst++;
        i++;
        src++;
    } while (i < 2);

    if (changed != 0) {
        raw = values[0];
        scale = g_EffectVolumeScale;
        left = raw * scale;
        raw = values[1];
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

        SsUtSetVVol(0x15, left, right);
        if (g_PanVoiceActive == 0) {
            right = 0xF;
            asm volatile("" : : "r"(right));
            voice = 0x15;
            asm volatile("" : : "r"(voice));
            raw = 0x3C;
            left = g_VabIds[0];
            zeroArg = 0;
            SsUtKeyOnV(voice, left, right, zeroArg, raw, 0, 0, 0);
        }
    } else if (g_PanVoiceActive != 0) {
        SsUtKeyOffV(0x15);
    }

    g_PanVoiceActive = changed;
}

extern s32 g_IndexedEffectIndex asm("D_801E6CF0");
extern s32 g_IndexedEffectIndexPrev asm("D_801E6CF4");
extern s32 g_IndexedEffectPitch asm("D_801E6CF8");
extern s32 g_IndexedEffectVolume asm("D_801E6CFC");
extern s32 g_IndexedEffectTones[] asm("D_800126AC");
extern s32 g_IndexedEffectVolumes[] asm("D_800126B4");

void GameStartIndexedEffectVoice(s32 baseTone) {
    SsUtKeyOnV(0x14, g_VabIds[0], (s16)baseTone, 0, 0x3C, 0, 0, 0);
}

void GameStopIndexedEffectVoice(void) {
    SsUtKeyOffV(0x14);
}

void GameSetIndexedEffectVoice(s32 index, s32 phase, s32 volume) {
    if (index >= -1) {
        if (index >= 3) {
            index = 2;
        }
    } else {
        index = -1;
    }

    if (volume >= 0) {
        if (volume >= 0x80) {
            volume = 0x7F;
        }
    } else {
        volume = 0;
    }

    g_IndexedEffectIndex = index;
    if (index >= 0) {
        g_IndexedEffectVolume = volume;
        g_IndexedEffectPitch = phase;
    }
}

void GameUpdateIndexedEffectVoice(void) {
    register s32 base asm("$16");
    register s32 center asm("$17");
    register s32 fine asm("$18");
    register s32 index asm("$3");
    register s32 raw asm("$2");
    register s32 product asm("$4");
    register s32 scale asm("$3");
    register s32 left asm("$5");
    register s32 right asm("$6");
    register s32 voice asm("$4");

    raw = g_IndexedEffectIndexPrev;
    if (raw < 0) {
        index = g_IndexedEffectIndex;
        if (index < 0) {
            goto update_state;
        }
        goto start_voice;
    } else {
        index = g_IndexedEffectIndex;
        if (index < 0) {
            GameStopIndexedEffectVoice();
        } else if (index != raw) {
        start_voice:
            raw = (index * 3) << 2;
            GameStartIndexedEffectVoice(*(s32 *)((s32)g_IndexedEffectTones + raw));
        }
    }

    raw = g_IndexedEffectIndex;
    if (raw >= 0) {
        index = (raw * 3) << 2;
        product = g_IndexedEffectVolume * *(s32 *)((s32)g_IndexedEffectVolumes + index);
        raw = g_IndexedEffectPitch;
        base = *(s32 *)((s32)g_IndexedEffectTones + index);
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

update_state:
    g_IndexedEffectIndexPrev = g_IndexedEffectIndex;
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/audio/GameSetPanVoiceTargetVolume", func_8005C31C);

void func_80078528(s32 voice, s16 left, s16 right);

#define UPDATE_BASIC_EFFECT_VOLUME()                                  \
    raw = *(s32 *)((u8 *)&g_MusicChannels[0].volLeft + offset);                              \
    scale = g_EffectVolumeScale;                                                \
    left = raw * scale;                                                \
    raw = *(s32 *)((u8 *)&g_MusicChannels[0].volRight + offset);                              \
    voice = i + 8;                                                     \
    if (left < 0) {                                                    \
        left += 0x7F;                                                  \
    }                                                                 \
    raw *= scale;                                                      \
    if (raw < 0) {                                                     \
        raw += 0x7F;                                                   \
    }                                                                 \
    left >>= 7;                                                        \
    right = raw >> 7;                                                  \
    if (left >= 0) {                                                   \
        if (left >= 0x81) {                                            \
            left = 0x80;                                               \
        }                                                             \
    } else {                                                          \
        left = 0;                                                      \
    }                                                                 \
    if (right >= 0) {                                                  \
        if (right >= 0x81) {                                           \
            right = 0x80;                                              \
        }                                                             \
    } else {                                                          \
        right = 0;                                                     \
    }                                                                 \
    func_80078528((s16)voice, (s16)left, (s16)right);                  \
    *state = neg

#define START_BASIC_EFFECT_VOLUME()                                   \
    raw = *(s32 *)((u8 *)&g_MusicChannels[0].volLeft + offset);                              \
    scale = g_EffectVolumeScale;                                                \
    left = raw * scale;                                                \
    raw = i + 8;                                                       \
    asm("" : "=r"(raw) : "0"(raw));                                    \
    voice = raw;                                                       \
    raw = *(s32 *)((u8 *)&g_MusicChannels[0].volRight + offset);                              \
    if (left < 0) {                                                    \
        left += 0x7F;                                                  \
    }                                                                 \
    raw *= scale;                                                      \
    if (raw < 0) {                                                     \
        raw += 0x7F;                                                   \
    }                                                                 \
    left >>= 7;                                                        \
    right = raw >> 7;                                                  \
    if (left >= 0) {                                                   \
        if (left >= 0x81) {                                            \
            left = 0x80;                                               \
        }                                                             \
    } else {                                                          \
        left = 0;                                                      \
    }                                                                 \
    if (right >= 0) {                                                  \
        if (right >= 0x81) {                                           \
            right = 0x80;                                              \
        }                                                             \
    } else {                                                          \
        right = 0;                                                     \
    }                                                                 \
    func_80078528((s16)voice, (s16)left, (s16)right);                  \
    *state = neg

void func_8005C6C0(void) {
    register s32 offset asm("$16");
    register s32 *state asm("$17");
    register s32 i asm("$18");
    register s32 voicePacked asm("$19");
    register s32 neg asm("$20");
    register s32 raw asm("$2");
    register s32 scale asm("$3");
    register s32 voice asm("$4");
    register s32 left asm("$5");
    register s32 right asm("$6");

    i = 0;
    neg = -1;
    state = &g_MusicChannels[0].mode;
    voicePacked = 0x80000;
    offset = 0;
    do {
        switch (*state) {
        case 0:
            SsUtKeyOnV(voicePacked >> 16, g_VabIds[0],
                          *(s16 *)((u8 *)&g_MusicChannels[0].left + offset),
                          *(s16 *)((u8 *)&g_MusicChannels[0].right + offset), 0x3C, 0, 0, 0);
            START_BASIC_EFFECT_VOLUME();
            break;
        case 2:
            UPDATE_BASIC_EFFECT_VOLUME();
            break;
        case 1:
            SsUtKeyOffV(voicePacked >> 16);
            *state = neg;
            break;
        }
        state = (s32 *)((s32)state + 0x18);
        voicePacked += 0x10000;
        i++;
        offset += 0x18;
    } while (i < 2);
}

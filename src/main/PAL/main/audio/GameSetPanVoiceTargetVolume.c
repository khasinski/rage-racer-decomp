#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"
#include "game/sound.h"

extern s32 g_StereoOutput asm("D_80082F40");
extern s32 g_PanVoiceVolumeL asm("D_801E6CE4");
extern s32 g_PanVoiceVolumeR asm("D_801E6CE8");

void GameSetPanVoiceTargetVolume(s32 arg0, s32 arg1) {
    if (arg0 >= 0) {
        if (arg0 > 0x80) {
            arg0 = 0x80;
        }
    } else {
        arg0 = 0;
    }

    if (arg1 >= 0) {
        if (arg1 > 0x80) {
            arg1 = 0x80;
        }
    } else {
        arg1 = 0;
    }

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

long SsUtKeyOffV(long voice) asm("func_80078018");

void GameApplyPanVoiceVolume(void) asm("func_8005BF30");
void GameApplyPanVoiceVolume(void) {
    s32 values[2];
    s32 changed;
    s32 i;
    s32 *dst;
    s32 *src;
    s32 raw;
    s32 loopValue;
    s32 scale;
    s32 left;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 right asm("$6");
    register s32 voice asm("$4");
    s32 zeroArg;

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
    s32 base;
    s32 center;
    s32 fine;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 index asm("$3");
    register s32 raw asm("$2");
    s32 product;
    s32 scale;
    /* These pins are load-bearing: removing any one changes .text. */
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

extern s32 D_800126D0[];
extern s32 D_801E6C9C;
extern u8 D_801E6D00[];
extern u8 D_801E6D04[];
extern s32 D_801E6D08;
extern u8 D_801E6D10[];
extern u8 D_801E6D14[];
extern s32 D_801E6D18;

typedef struct SoundModeSlot {
    s32 left;
    s32 right;
} SoundModeSlot;

typedef struct SoundModeEntry {
    s32 count;
    s32 factor;
    SoundModeSlot slots[2];
} SoundModeEntry;

void func_8005C31C(s32 arg0, s32 left, s32 right) {
    s32 offset;
    s32 count;
    s32 i;
    s32 loopTableOffset;
    s32 average;
    /* Load-bearing: removing this $v0 pin changes four linked words. */
    register s32 scaledLeft asm("$2");
    s32 scaledRight;
    s32 entryOffset;
    s32 *table;
    s32 currentA;
    s32 currentB;
    s32 matchValue;
    s32 flag;
    s32 *base;
    SoundModeEntry *entry;

    if (arg0 < 0) {
        goto clamp_arg0_low;
    }
    if (arg0 < 4) {
        goto clamp_arg0_done;
    }
    arg0 = 3;
    goto clamp_arg0_done;
clamp_arg0_low:
    arg0 = 0;
clamp_arg0_done:

    if (left < 0) {
        goto clamp_left_low;
    }
    if (left < 0x80) {
        goto clamp_left_done;
    }
    left = 0x7F;
    goto clamp_left_done;
clamp_left_low:
    left = 0;
clamp_left_done:

    if (right < 0) {
        goto clamp_right_low;
    }
    if (right < 0x80) {
        goto clamp_right_done;
    }
    right = 0x7F;
    goto clamp_right_done;
clamp_right_low:
    right = 0;
clamp_right_done:

    if ((left <= 0) && (right <= 0)) {
        left = *(s32 *)D_801E6D00;
        right = 0;
        if (left < 0) {
            if (D_801E6D18 < 0) {
                return;
            }
        }

        if ((u32)arg0 < 2) {
            if (left == *(s32 *)(D_800126D0 + 2)) {
                currentB = D_801E6D18;
                if (currentB == *(s32 *)(D_800126D0 + 4)) {
                    goto found_match;
                }
            }
            if (left == *(s32 *)(D_800126D0 + 8)) {
                currentB = D_801E6D18;
                matchValue = *(s32 *)(D_800126D0 + 10);
                goto test_match;
            }
        } else {
            if (left == *(s32 *)(D_800126D0 + 14)) {
                currentB = D_801E6D18;
                if (currentB == *(s32 *)(D_800126D0 + 16)) {
                    goto found_match;
                }
            }
            if (left == *(s32 *)(D_800126D0 + 20)) {
                currentB = D_801E6D18;
                matchValue = *(s32 *)(D_800126D0 + 22);
                goto test_match;
            }
        }
        goto after_match;
test_match:
        if (currentB != matchValue) {
            goto after_match;
        }
found_match:
        right = 1;
after_match:

        if (right != 0) {
            /* Load-bearing: removing this $v0 pin changes 139 linked words. */
            register s32 resetLoad;
            s32 resetCount;
            s32 inactiveValue;
            s32 activeValue;

            resetLoad = *(s32 *)((s32)D_800126D0 + ((arg0 * 3) << 3));
            i = 0;
            if (resetLoad <= i) {
                return;
            }
            inactiveValue = -1;
            activeValue = 1;
            resetCount = resetLoad;
            do {
                offset = i * 0x18;
                *(s32 *)(D_801E6D00 + offset) = inactiveValue;
                *(s32 *)(D_801E6D04 + offset) = inactiveValue;
                *(s32 *)((s32)&D_801E6D08 + offset) = activeValue;
                table = (s32 *)((s32)&D_801E6C9C + offset);
                *(s32 *)((s32)table + 0x78) = 0;
                *(s32 *)((s32)D_801E6D10 + offset) = 0;
                i++;
            } while (i < resetCount);
        }
        return;
    }

    currentA = *(s32 *)D_801E6D00;
    if (currentA == *(s32 *)((s32)D_800126D0 + ((arg0 * 3) << 3) + 8)) {
        currentB = D_801E6D18;
        if (currentB == *(s32 *)((s32)D_800126D0 + ((arg0 * 3) << 3) + 0x10)) {
            D_801E6D08 = 2;
        } else {
            D_801E6D08 = 0;
        }
    } else {
        D_801E6D08 = 0;
    }

    i = 0;
    loopTableOffset = (arg0 * 3) << 3;
    arg0 = *(s32 *)((s32)D_800126D0 + loopTableOffset);
    if (arg0 <= i) {
        return;
    }

    average = (left + right) / 2;
    count = arg0;
    /* Load-bearing: removal changes five linked preheader words. */
    asm("" : "=r"(count) : "0"(count));
    base = D_800126D0;
    entryOffset = loopTableOffset;
    entry = (SoundModeEntry *)((s32)base + entryOffset);
    arg0 = 0;
    do {
        if (i != 0) {
            *(s32 *)((s32)&D_801E6D08 + arg0) = D_801E6D08;
        }

        flag = g_StereoOutput;
        *(s32 *)(D_801E6D00 + arg0) = entry->slots[0].left;
        *(s32 *)(D_801E6D04 + arg0) = entry->slots[0].right;
        if (flag != 0) {
            currentB = *(s32 *)((s32)D_800126D0 + entryOffset + 4);
            scaledLeft = left * currentB;
            if (scaledLeft < 0) {
                scaledLeft += 0x7F;
            }
            scaledLeft >>= 7;
            *(volatile s32 *)((s32)D_801E6D10 + arg0) = scaledLeft;
            scaledRight = right * currentB;
            entry = (SoundModeEntry *)((s32)entry + 8);
            if (scaledRight < 0) {
                scaledRight += 0x7F;
            }
            scaledRight >>= 7;
            *(volatile s32 *)((s32)D_801E6D14 + arg0) = scaledRight;
            i++;
        } else {
            if ((scaledLeft = average * *(s32 *)((s32)D_800126D0 + entryOffset + 4)) < 0) {
                currentB = scaledLeft + 0x7F;
            } else {
                currentB = scaledLeft;
            }
            currentB >>= 7;
            *(volatile s32 *)((s32)D_801E6D10 + arg0) = currentB;
            *(volatile s32 *)((s32)D_801E6D14 + arg0) = currentB;
            /* Load-bearing: removal changes eight linked scheduler words. */
            asm volatile("");
            entry = (SoundModeEntry *)((s32)entry + 8);
            i++;
        }
        arg0 += 0x18;
    } while (i < count);
}

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
    s32 offset;
    s32 *state;
    s32 i;
    s32 voicePacked;
    s32 neg;
    s32 raw;
    s32 scale;
    s32 voice;
    s32 left;
    s32 right;

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

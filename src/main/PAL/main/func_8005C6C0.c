#include "common.h"
#include "game/audio.h"
#include "game/sound.h"
#include "psyq/snd.h"


void func_80078528(s32 voice, s16 left, s16 right);
s32 SsUtKeyOffV(s32 voice) asm("func_80078018");

#define UPDATE_BASIC_EFFECT_VOLUME()                                  \
    raw = *(s32 *)((u8 *)&D_801E6D00[0].volLeft + offset);                              \
    scale = g_EffectVolumeScale;                                                \
    left = raw * scale;                                                \
    raw = *(s32 *)((u8 *)&D_801E6D00[0].volRight + offset);                              \
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
    raw = *(s32 *)((u8 *)&D_801E6D00[0].volLeft + offset);                              \
    scale = g_EffectVolumeScale;                                                \
    left = raw * scale;                                                \
    raw = i + 8;                                                       \
    asm("" : "=r"(raw) : "0"(raw));                                    \
    voice = raw;                                                       \
    raw = *(s32 *)((u8 *)&D_801E6D00[0].volRight + offset);                              \
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
    state = &D_801E6D00[0].mode;
    voicePacked = 0x80000;
    offset = 0;
    do {
        switch (*state) {
        case 0:
            SsUtKeyOnV(voicePacked >> 16, g_VabIds[0],
                          *(s16 *)((u8 *)&D_801E6D00[0].left + offset),
                          *(s16 *)((u8 *)&D_801E6D00[0].right + offset), 0x3C, 0, 0, 0);
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

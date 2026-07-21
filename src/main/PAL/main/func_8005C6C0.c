#include "common.h"

extern s32 D_801E6D08;
extern s16 D_801E6CA8;
extern u8 D_801E6D00[];
extern u8 D_801E6D04[];
extern u8 D_801E6D10[];
extern u8 D_801E6D14[];
extern s32 D_801E6CA4;

void func_80077C7C(s32 voice, s32 vab, s32 prog, s32 tone, s32 note, s32 a5, s32 a6, s32 a7);
void func_80078528(s32 voice, s16 left, s16 right);
void func_80078018(s32 voice);

#define UPDATE_BASIC_EFFECT_VOLUME()                                  \
    raw = *(s32 *)(D_801E6D10 + offset);                              \
    scale = D_801E6CA4;                                                \
    left = raw * scale;                                                \
    raw = *(s32 *)(D_801E6D14 + offset);                              \
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
    raw = *(s32 *)(D_801E6D10 + offset);                              \
    scale = D_801E6CA4;                                                \
    left = raw * scale;                                                \
    raw = i + 8;                                                       \
    asm("" : "=r"(raw) : "0"(raw));                                    \
    voice = raw;                                                       \
    raw = *(s32 *)(D_801E6D14 + offset);                              \
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
    state = &D_801E6D08;
    voicePacked = 0x80000;
    offset = 0;
    do {
        switch (*state) {
        case 0:
            func_80077C7C(voicePacked >> 16, D_801E6CA8,
                          *(s16 *)(D_801E6D00 + offset),
                          *(s16 *)(D_801E6D04 + offset), 0x3C, 0, 0, 0);
            START_BASIC_EFFECT_VOLUME();
            break;
        case 2:
            UPDATE_BASIC_EFFECT_VOLUME();
            break;
        case 1:
            func_80078018(voicePacked >> 16);
            *state = neg;
            break;
        }
        state = (s32 *)((s32)state + 0x18);
        voicePacked += 0x10000;
        i++;
        offset += 0x18;
    } while (i < 2);
}

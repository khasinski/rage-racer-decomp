#include "common.h"

extern s32 D_801E6D38[];
extern u8 D_801E6D34[];
extern u8 D_801E6D40[];
extern s32 D_801E6CA4;
extern s16 D_801E6CA8;

void func_80077C7C(s32, s32, s32, s32, s32, s32, s32, s32);
void func_80078018(s32);
void func_80078528(s32, s16, s16);
void func_800781C0(s32, s32, s32, s32, s32, s32, s32);

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005C914", func_8005C914);

/* Loop over the 4 effect voices (indices 10..13). `voice` is kept in the
 * compiler's scaled (<<16) representation of the short voice number, exactly
 * as GCC materialises it, and read back with `voice >> 16`. */
#define VOLPITCH()                                                    \
    svArg = voiceCopy;                                                \
    asm("" : "=r"(svArg) : "0"(svArg));                               \
    prod = *(s32 *)(D_801E6D40 + offset) * D_801E6CA4;                \
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
    statePtr = D_801E6D38;
    voice = 10 << 16;
    voiceCopy = 10;
    pitchPtr = statePtr + 1;
    f0Ptr = (s16 *)statePtr - 4;
    offset = 0;
    do {
        state = *statePtr;
        switch (state) {
        case 0:
            func_80077C7C(voice >> 16, D_801E6CA8, *f0Ptr,
                          *(s16 *)(D_801E6D34 + offset), 0x3C, 0, 0, 0);
            VOLPITCH();
            break;
        case 2:
            VOLPITCH();
            break;
        case 1:
            func_80078018(voice >> 16);
            *statePtr = neg;
            break;
        }
        statePtr = (s32 *)((u8 *)statePtr + 0x14);
        voice += 1 << 16;
        voiceCopy++;
        pitchPtr = (s32 *)((u8 *)pitchPtr + 0x14);
        f0Ptr = (s16 *)((u8 *)f0Ptr + 0x14);
        offset += 0x14;
    } while ((s32)statePtr < (s32)&D_801E6D38[20]);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005C914", func_8005D050);

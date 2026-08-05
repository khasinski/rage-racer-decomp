#include "common.h"
#include "game/state.h"

extern u8 g_PadBuffers[] asm("D_801E403C");

void InitPad(void *buf0, s32 len0, void *buf1, s32 len1);
void StartPad(void);

/* BIOS InitPAD over the two 0x28-byte pad buffers, then StartPAD. */
void GameInitPad(void) {
    InitPad(g_PadBuffers, 0x28, g_PadBuffers + 0x28, 0x28);
    StartPad();
}

/* Eight selectable button-mapping presets per controller, one row of eight
 * button masks each: g_PadButtonPresets for the standard pad, g_NegconButtonPresets for the
 * NeGcon. */
extern u16 g_PadButtonPresets[] asm("D_8007C028");
extern u16 g_NegconButtonPresets[] asm("D_8007C0A8");
/* The live mapping UpdatePadState reads: the pad's eight masks at +0,
 * the NeGcon's eight at +0x10. */
extern u16 g_PadButtonMapping[] asm("D_801E4B60");

/*
 * Installs the two selected presets into the live mapping table. Both rows are
 * copied in the same 8-iteration loop, hence the pair of source and
 * destination cursors.
 */
void LoadPadButtonMapping(s32 mapping0, s32 mapping1) {
    s32 i;
    u16 *dst0;
    u16 *dst1;
    u16 *src0;
    u16 *src1;
    u16 *table;

    i = 0;
    dst0 = g_PadButtonMapping;
    dst1 = dst0 + 8;
    table = g_NegconButtonPresets;
    src1 = table + mapping1 * 8;
    table = g_PadButtonPresets;
    src0 = table + mapping0 * 8;

    do {
        u16 mask;

        *dst0 = *src0++;
        i++;
        mask = *src1++;
        dst0++;
        *dst1++ = mask;
    } while (i < 8);
}

extern s16 g_PadMappingIndex asm("D_8019CE08");
extern s16 g_NegconMappingIndex asm("D_8019CB08");

/* Re-applies the button mapping from the two saved selections. */
void ApplyPadButtonMapping(void) {
    LoadPadButtonMapping(g_PadMappingIndex, g_NegconMappingIndex);
}

typedef struct PadState {
    /* 0x00 */ u8  unk0;
    /* 0x01 */ u8  unk1;
    /* 0x02 */ u16 held;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 pressed;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
} PadState;

extern PadState g_PadState asm("D_801E4368");
extern u8 g_PadType asm("D_801E4369");
extern u8 g_PadBufferType asm("D_801E403D");
extern u8 g_PadBufferButtonsHigh asm("D_801E403E");
extern u8 g_PadBufferButtonsLow asm("D_801E403F");
extern s32 D_801E4D14;
extern s32 g_PadErrorState asm("D_801E79C8");
extern s32 D_8019CB10;
extern u16 g_PadPrevHeld asm("D_8007C138");
extern u8 g_PadRepeatTimer[] asm("D_8009AEEC");
extern u16 D_8007C128[][2];
extern s16 g_NegconSteerPlay asm("D_8019CAD0");
extern s16 g_NegconSteerRange[] asm("D_8007C020");
extern s16 g_NegconMaxTwist asm("D_801E418C");
extern s16 g_NegconNeutralI asm("D_8019CA08");
extern s16 g_NegconNeutralII asm("D_8019CA0A");
extern s16 g_NegconNeutralL asm("D_8019CA0C");
extern s16 g_NegconSteerNeutral asm("D_801E4BF0");

void UpdatePadState(void);
void UpdatePadState(void) {
    s32 mask;
    s32 v;
    s32 t;
    s32 n;
    s32 c;
    s16 c1;
    s16 c2;
    s16 d;
    s16 neutral;
    s16 r;
    u8 *raw;
    PadState *pad;

    raw = g_PadBuffers;
    pad = &g_PadState;

    pad->unk0 = raw[0];
    g_PadType = g_PadBufferType;
    if (raw[0] != 0) {
        v = 1;
        g_PadErrorState = v;
        D_801E4D14 = 0x22;
        D_8019CB10 |= 0x10;
    } else {
        if (D_801E4D14 != 0) {
            D_801E4D14 = D_801E4D14 - 1;
            if (g_PadBufferType == 0x23) {
                mask = ~(g_PadBufferButtonsLow | (g_PadBufferButtonsHigh << 8));
                if (!(((mask & 0x5000) != 0x5000) && ((mask & 0xA000) != 0xA000) &&
                    ((mask & 0x1C4) == 0))) {
                    v = 2;
        g_PadErrorState = v;
        D_801E4D14 = 0x22;
        D_8019CB10 |= 0x10;
                }
            }
        }
    }
    D_8019CB10 = D_8019CB10 >> 1;
    if (D_8019CB10 != 0) {
        raw[1] = 0;
        pad->unk1 = 0;
    } else {
        g_PadErrorState = 0;
    }
    if (raw[1] == 0x41) {
        pad->unk4 = pad->held;
        pad->held = ~((raw[2] << 8) | raw[3]);
        pad->unk6 = pad->held & ~pad->unk4;
        t = (pad->held >> 13) & 1;
        n = t;
        c = g_NegconSteerRange[g_NegconMaxTwist];
        pad->unkA = ((pad->held & 0x8000) ? ((n - 1) * c) : (n * c)) + 0x80;
        pad->unkC = (pad->held & 0x40) ? 0x6A : 0;
        pad->unkE = (pad->held & 0x80) ? 0x6A : 0;
        pad->unk10 = (pad->held & 0x4) ? 0x6A : 0;
    } else if (raw[1] == 0x23) {
        pad->unk4 = pad->held;
        pad->held = ~((raw[2] << 8) | raw[3]);
        pad->unkA = raw[4];
        asm("");
        pad->unkC = raw[5] - g_NegconNeutralI;
        pad->unkE = raw[6] - g_NegconNeutralII;
        pad->unk10 = raw[7] - g_NegconNeutralL;
        if (pad->unkC < 0) {
            pad->unkC = 0;
        }
        if (pad->unkE < 0) {
            pad->unkE = 0;
        }
        if (pad->unk10 < 0) {
            pad->unk10 = 0;
        }
        if (pad->unkA >= 0xA3) {
            pad->held |= 0x2000;
        }
        if (pad->unkA < 0x5E) {
            pad->held |= 0x8000;
        }
        if (pad->unkC >= 0x36) {
            pad->held |= 0x40;
        }
        if (pad->unkE >= 0x36) {
            pad->held |= 0x80;
        }
        if (pad->unk10 >= 0x36) {
            pad->held |= 0x4;
        }
        pad->unk6 = pad->held & ~pad->unk4;
    } else {
        if (g_PadErrorState == 0) {
            g_PadErrorState = 2;
        }
        pad->unk0 = 1;
        pad->held = 0;
        pad->unk6 = 0;
        pad->pressed = 0;
        pad->unkA = 0x80;
        pad->unkC = 0;
        pad->unkE = 0;
        pad->unk10 = 0;
    }
    pad->pressed = 0;
    pad->pressed = pad->held & ~g_PadPrevHeld;
    if (pad->held != 0 && pad->held == g_PadPrevHeld) {
        if (g_PadRepeatTimer[0] == 0x1E) {
            pad->pressed = pad->pressed | pad->held;
        }
        g_PadRepeatTimer[0] = (g_PadRepeatTimer[0] < 0x24) ? (g_PadRepeatTimer[0] + 1) : 0x1E;
    } else {
        g_PadRepeatTimer[0] = 0;
    }
    g_PadPrevHeld = pad->held;
    neutral = g_NegconSteerNeutral + 0x80;
    d = pad->unkA - neutral;
    if (d > 0) {
        r = d - D_8007C128[g_NegconSteerPlay][0];
        if (r < 0) {
            r = 0;
        }
        c1 = g_NegconSteerRange[g_NegconMaxTwist];
        if (r > c1) {
            r = c1;
        }
    } else {
        r = d + D_8007C128[g_NegconSteerPlay][0];
        if (r > 0) {
            r = 0;
        }
        c2 = g_NegconSteerRange[g_NegconMaxTwist];
        if (r < -c2) {
            r = -c2;
        }
    }
    pad->unk16 = r;
    if (pad->unkC >= 0x6B) {
        pad->unkC = 0x6A;
    }
    if (pad->unkE >= 0x6B) {
        pad->unkE = 0x6A;
    }
    if (pad->unk10 >= 0x6B) {
        pad->unk10 = 0x6A;
    }
}

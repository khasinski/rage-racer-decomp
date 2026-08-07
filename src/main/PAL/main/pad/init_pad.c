#include "common.h"
#include "game/state.h"
#include "game/render.h"

/* BIOS InitPAD over the two 0x28-byte pad buffers, then StartPAD. */
void GameInitPad(void) {
    InitPad(g_PadBuffers, 0x28, g_PadBuffers + 0x28, 0x28);
    StartPad();
}

/* The live mapping UpdatePadState reads: the pad's eight masks at +0,
 * the NeGcon's eight at +0x10. */
extern u16 g_PadButtonMapping[];

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

extern s16 g_PadMappingIndex;
extern s16 g_NegconMappingIndex;

/* Re-applies the button mapping from the two saved selections. */
void ApplyPadButtonMapping(void) {
    LoadPadButtonMapping(g_PadMappingIndex, g_NegconMappingIndex);
}

extern u8 g_PadType;
extern s16 g_NegconSteerPlay;
extern s16 g_NegconMaxTwist;
extern s16 g_NegconNeutralI;
extern s16 g_NegconNeutralII;
extern s16 g_NegconNeutralL;
extern s16 g_NegconSteerNeutral;

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

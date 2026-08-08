#include "common.h"
#include "game/race.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "psyq/gpu.h"


void DrawRaceHudLabels(s32 mode) {
    s32 count;
    s32 i;
    s32 offset;
    void **scratch;

    count = 9;
    if (mode != 0) {
        count = 0xC;
    }

    i = 6;
    if (i < count) {
        offset = 0x23770;
        do {
            u8 *base = g_DrawBuffer;

            i++;
            AddPrim(base + 0xCC, (void *)(offset + (s32)base));
            offset += 0x14;
        } while (i < count);
    }

    scratch = &SCRATCH_PRIM_CURSOR_AS(void);
    *scratch = QueueDrawModePrim(g_DrawBuffer + 0xCC, *scratch, 9);
}

/*
 * Packs a TILE (SetTile + AddPrim) at `prim`, links it into `ot` and returns
 * the cursor advanced past the 0x10-byte packet. Declared per translation unit
 * rather than in a header: callers disagree on whether `ot`/`prim` are pointers
 * or s32, and gcc 2.6.3 will not take both against one prototype.
 */
u8 *AddTilePrim(void *ot, u8 *prim, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b) {
    u8 *oldPrim;

    SetTile(prim);

    oldPrim = prim;
    *(s16 *)(prim + 0x08) = x;
    *(s16 *)(prim + 0x0A) = y;
    *(s16 *)(prim + 0x0C) = w;
    *(s16 *)(prim + 0x0E) = h;
    prim[0x04] = r;
    prim[0x05] = g;
    prim[0x06] = b;

    prim += 0x10;
    AddPrim(ot, oldPrim);
    return prim;
}

/* Expands a GameSpriteDesc into a scratchpad SPRT. */
void BuildSpriteFromDesc(SPRT *prim, GameSpriteDesc *src) {
    SetSprt(prim);

    prim->x0 = src->x;
    prim->y0 = src->y;
    prim->w = src->w;
    prim->h = src->h;
    prim->u0 = src->u0;
    prim->v0 = src->v0;
    prim->clut = src->clut;
    SetSemiTrans(prim, src->semiTrans);
    SetShadeTex(prim, 1);
}

extern GameRaceRanking g_PlayerLap;
void DrawTimeValue(s32 x, s32 y, s32 value, s32 color, s32 divisor);

/* The lap-time column: one row per lap from g_PlayerLap.values at x=0xFA,
 * y stepping 0xA, the current lap highlighted and unset laps drawn as -1. */
void DrawLapTimes(void) {
    s32 i;
    s32 visibleCount;
    s32 activeIndex;
    s32 tile;
    s32 y;
    s32 primOffset;
    s32 baseOffset;
    s32 *valuePtr;
    GameRaceRanking *list;
    u8 *base;
    void *ot;
    void *prim;
    s32 framePad[2];
    s32 value;

    list = &g_PlayerLap;
    visibleCount = list->count;
    if (visibleCount > g_LapCount) {
        visibleCount = g_LapCount;
    }

    i = 0;
    activeIndex = D_8009E836;
    if (g_LapCount > 0) {
        baseOffset = 0x236F8;
        primOffset = 0;
        y = 0x2E;
        valuePtr = list->values;

        do {
            if (i == activeIndex) {
                tile = 0x780F;
            } else if (valuePtr[0] > 0x927BE) {
                tile = 0x7890;
            } else {
                tile = 0x78CC;
            }

            if (i < visibleCount) {
                value = valuePtr[0];
            } else {
                value = -1;
            }

            DrawTimeValue(0xFA, y, value, tile, 0x3E8);
            y += 0xA;
            valuePtr++;
            base = g_DrawBuffer;
            ot = base + 0xCC;
            prim = (void *)(baseOffset + (s32)base);
            *(s16 *)(primOffset + (s32)base + 0x23706) = tile;
            AddPrim(ot, prim);
            i++;
            baseOffset += 0x14;
            primOffset += 0x14;
        } while (i < g_LapCount);
    }

    DrawTimeValue(0xFA, 0x20, g_BestLapThisRace, 0x78CC, 0x3E8);
}

void DrawTimeRemaining(s32 time) {
    s32 clutIndex = 0x78CC;

    if (time < 0x5DC) {
        clutIndex = 0x7811;
    }

    DrawMinuteSecondTime(0xE, 0xD2, time, clutIndex);
}

/* The two race-position digits, from g_RacePosition; the tens digit is
 * blanked below 10 and the colour changes from 4th place down. */
void DrawRacePosition(void) {
    u8 *base;
    s32 value;
    u8 *left;
    u8 *right;

    base = g_DrawBuffer;
    value = g_RacePosition;
    left = base + 0x237AC;
    right = base + 0x237C0;

    if (value >= 10) {
        *(u8 *)(base + 0x237B8) = 0x18;
    } else {
        *(u8 *)(base + 0x237B8) = 0;
    }

    {
        s32 quotient;
        s32 digit;

        quotient = value / 10;
        digit = (value - quotient * 10) * 24;
        *(u8 *)(right + 0xC) = digit;
    }

    if (value < 4) {
        *(s16 *)(left + 0xE) = 0x780B;
        *(s16 *)(right + 0xE) = 0x780B;
    } else {
        *(s16 *)(left + 0xE) = 0x780E;
        *(s16 *)(right + 0xE) = 0x780E;
    }
}

void SetHudBlinkColor(s32 phase) {
    *(u16 *)(g_DrawBuffer + 0x237A6) = phase ? 0x7811 : 0x7800;
}

void DrawSplitDelta(s32 delta, s32 y) {
    u8 *base;
    register u8 *prim;
    s32 firstOffset;
    s32 value;
    s32 temp;
    u8 *ot;

    firstOffset = 0x237AC;
    temp = 0x237C0;
    value = delta * 8;
    base = g_DrawBuffer;
    value += 0x50;
    prim = base + temp;

    *(base + 0x237B8) = value;
    AddPrim(g_DrawBuffer + 0xCC, base + firstOffset);
    firstOffset = (s32)prim;

    if (y > 0) {
        *(volatile u8 *)(base + 0x237CC) = 0x88;
        ot = g_DrawBuffer;
        __asm__ volatile("" : : "r"(ot), "r"(prim));
        temp = 0x7810;
    } else if (y < 0) {
        *(volatile u8 *)(base + 0x237CC) = 0x78;
        ot = g_DrawBuffer;
        __asm__ volatile("" : : "r"(ot));
        temp = 0x780F;
    } else {
        return;
    }

    *(u16 *)(base + 0x237CE) = temp;
    AddPrim(ot + 0xCC, (void *)firstOffset);
}

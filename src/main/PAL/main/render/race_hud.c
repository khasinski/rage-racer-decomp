#include "common.h"
#include "game/prim.h"
#include "game/race.h"
#include "game/player_car_internal.h"
#include "game/render.h"
#include "game/render_internal.h"
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
            RenderBufferAddress base;
            RenderBufferAddress prim;

            base.bytes = g_DrawBuffer;
            i++;
            prim.byteOffset = offset + base.byteOffset;
            AddPrim(base.bytes + 0xCC, prim.pointer);
            offset += 0x14;
        } while (i < count);
    }

    scratch = &SCRATCH_PRIM_CURSOR_AS(void);
    *scratch = QueueDrawModePrim(g_DrawBuffer + 0xCC, *scratch, 9);
}

u8 *AddTilePrim(void *ot, u8 *prim, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b) {
    RenderBufferAddress cursor;
    TILE *tile;
    u8 *oldPrim;

    cursor.bytes = prim;
    tile = cursor.tile;
    SetTile(tile);

    oldPrim = prim;
    tile->x0 = x;
    tile->y0 = y;
    tile->w = w;
    tile->h = h;
    tile->t.r0 = r;
    tile->t.g0 = g;
    tile->t.b0 = b;

    prim += sizeof(*tile);
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


/* The lap-time column: one row per lap from the player timing table at x=0xFA,
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
    RenderBufferAddress base;
    void *ot;
    RenderBufferAddress prim;
    RenderBufferAddress sprite;
    s32 framePad[2];
    s32 value;

    visibleCount = g_PlayerCar.lap;
    if (visibleCount > g_LapCount) {
        visibleCount = g_LapCount;
    }

    i = 0;
    activeIndex = g_HudLapHighlightRow;
    if (g_LapCount > 0) {
        baseOffset = 0x236F8;
        primOffset = 0;
        y = 0x2E;
        valuePtr = g_PlayerCar.lapTimes.table.milliseconds;

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
            base.bytes = g_DrawBuffer;
            ot = base.bytes + 0xCC;
            prim.byteOffset = baseOffset + base.byteOffset;
            sprite.byteOffset = primOffset + base.byteOffset + 0x236F8;
            sprite.sprite->clut = tile;
            AddPrim(ot, prim.pointer);
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
    GameFrameContextAddress drawBuffer;
    u8 *base;
    s32 value;
    SPRT *left;
    SPRT *right;

    base = g_DrawBuffer;
    drawBuffer.bytes = base;
    value = g_RacePosition;
    left = &drawBuffer.context->layout.raceHud.labels[3];
    right = &drawBuffer.context->layout.raceHud.labels[4];

    if (value >= 10) {
        left->u0 = 0x18;
    } else {
        left->u0 = 0;
    }

    {
        s32 quotient;
        s32 digit;

        quotient = value / 10;
        digit = (value - quotient * 10) * 24;
        right->u0 = digit;
    }

    if (value < 4) {
        left->clut = 0x780B;
        right->clut = 0x780B;
    } else {
        left->clut = 0x780E;
        right->clut = 0x780E;
    }
}

void SetHudBlinkColor(s32 phase) {
    GameFrameContextAddress drawBuffer;

    drawBuffer.bytes = g_DrawBuffer;
    drawBuffer.context->layout.raceHud.labels[2].clut = phase ? 0x7811 : 0x7800;
}

void DrawSplitDelta(s32 delta, s32 y) {
    GameFrameContextAddress drawBuffer;
    u8 *base;
    u8 *prim;
    RenderBufferAddress firstPrim;
    s32 value;
    s32 temp;
    u8 *ot;

    firstPrim.byteOffset = 0x237AC;
    temp = 0x237C0;
    value = delta * 8;
    base = g_DrawBuffer;
    drawBuffer.bytes = base;
    value += 0x50;
    prim = base + temp;

    drawBuffer.context->layout.raceHud.labels[3].u0 = value;
    AddPrim(g_DrawBuffer + 0xCC, base + firstPrim.byteOffset);
    firstPrim.bytes = prim;

    if (y > 0) {
        ((volatile SPRT *)prim)->u0 = 0x88;
        ot = g_DrawBuffer;
        __asm__ volatile("" : : "r"(ot), "r"(prim));
        temp = 0x7810;
    } else if (y < 0) {
        ((volatile SPRT *)prim)->u0 = 0x78;
        ot = g_DrawBuffer;
        __asm__ volatile("" : : "r"(ot));
        temp = 0x780F;
    } else {
        return;
    }

    drawBuffer.context->layout.raceHud.labels[4].clut = temp;
    AddPrim(ot + 0xCC, firstPrim.pointer);
}

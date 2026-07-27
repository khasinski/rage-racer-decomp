#include "common.h"

/* Static text buffers the two time drawers format into, seeded in .data with
 * "0'00\"000" and "00'00\"". The named split symbols are the digit positions
 * each writer touches; the separators are never rewritten. */
extern u8 g_TimeTextBuffer[] asm("D_8007DF04");

void func_80016754(s32 x, s32 y, void *str, s32 color);

void GameDrawTimeValue(s32 x, s32 y, s32 value, s32 color, s32 divisor) asm("func_80033D50");
void GameDrawTimeValue(s32 x, s32 y, s32 value, s32 color, s32 divisor) {
    s32 savedX;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 savedY asm("$10");
    s32 savedColor;
    register s32 localDivisor asm("$4");
    s32 whole;
    s32 fraction;
    s32 minutes;
    s32 seconds;
    s32 secondTens;
    s32 fractionHundreds;
    s32 fractionTens;
    s32 remainder;

    savedX = x;
    savedY = y;
    localDivisor = divisor;
    savedColor = color;
    if (value >= 0) {
        whole = value / localDivisor;
        remainder = value % localDivisor;

        minutes = whole / 60;
        fraction = (remainder * 1000) / localDivisor;
        seconds = whole % 60;
        g_TimeTextBuffer[0] = minutes + '0';

        secondTens = seconds / 10;
        g_TimeTextBuffer[2] = secondTens + '0';
        g_TimeTextBuffer[3] = (seconds - (secondTens * 10)) + '0';

        fractionHundreds = fraction / 100;
        g_TimeTextBuffer[5] = fractionHundreds + '0';

        fractionTens = fraction / 10;
        g_TimeTextBuffer[6] = (fractionTens - (fractionHundreds * 10)) + '0';
        g_TimeTextBuffer[7] = (fraction - (fractionTens * 10)) + '0';
    } else {
        g_TimeTextBuffer[0] = '-';
        g_TimeTextBuffer[2] = '-';
        g_TimeTextBuffer[3] = '-';
        g_TimeTextBuffer[5] = '-';
        g_TimeTextBuffer[6] = '-';
        g_TimeTextBuffer[7] = '-';
    }

    func_80016754(savedX, savedY, g_TimeTextBuffer, savedColor);
}

extern u8 g_ClockTextBuffer asm("D_8007DF10");
extern u8 g_ClockTextMinUnits[] asm("D_8007DF11");
extern u8 g_ClockTextSecTens asm("D_8007DF13");
extern u8 g_ClockTextSecUnits asm("D_8007DF14");

void GameDrawMinuteSecondTime(s32 x, s32 y, s32 ticks, s32 color) asm("func_80033F30");
void GameDrawMinuteSecondTime(s32 x, s32 y, s32 ticks, s32 color) {
    s32 savedY;
    s32 sec;
    s32 tmp;
    s32 min;
    u8 *p;
    s32 tens;
    s32 tens2;

    savedY = y;
    sec = ticks / 25;
    tmp = sec / 60;
    min = tmp;
    tmp = sec - min * 60;
    if (min < 10) {
        g_ClockTextBuffer = ' ';
    } else {
        g_ClockTextBuffer = min / 10 + '0';
    }
    tens = min / 10;
    p = g_ClockTextMinUnits;
    tens2 = tmp / 10;
    *p = min - tens * 10 + '0';
    g_ClockTextSecTens = tens2 + '0';
    g_ClockTextSecUnits = tmp - tens2 * 10 + '0';
    func_80016754(x, savedY, p - 1, color);
}

/* Which of the two frame buffers is being drawn, 0 or 1; the main loop sets
 * it beside g_DrawBuffer. Here it turns into the 240-line y bias of the
 * drawing-area rect. */
extern s32 g_FrameParity asm("D_801E4B34");

void func_80066604(void *packet, void *rect);
void AddPrim(void *ot, void *prim) asm("func_80064DDC");

void *GameQueueDrawAreaPrim(void *ot, void *packet, s16 x, s16 y, s32 w, s32 h) asm("func_80034058");
void *GameQueueDrawAreaPrim(void *ot, void *packet, s16 x, s16 y, s32 w, s32 h) {
    void *oldPacket;
    s16 rect[4];
    s32 offset;

    offset = ((g_FrameParity << 4) - g_FrameParity) << 4;
    rect[0] = x;
    rect[1] = y + offset;
    rect[2] = w;
    rect[3] = h;
    func_80066604(packet, rect);
    oldPacket = packet;
    packet = (u8 *)packet + 12;
    AddPrim(ot, oldPacket);
    return packet;
}

/* The two 12000-byte TILE strips func_800340D8 builds, backed by
 * g_TileStripStorage. Nothing in the decompiled tree reads them back, so what
 * the strip renders is still unestablished -- see docs/names.md 15g. */
extern u8 *g_TileStripBuffers[2] asm("D_8019C90C");
extern u8 g_TileStripStorage[] asm("D_801EB9DC");

void func_800658FC(s32 mode);
void func_80064FF8(u8 *prim);

void func_800340D8(void) {
    u8 **initBuffers;
    u8 **buffers;
    s32 row;
    s32 col;
    s32 linear;
    s32 offset;
    s32 y;
    s32 color;
    s32 xStep;
    s32 yStart;
    s32 bufferIndex;
    u8 *buffer;
    u8 *firstBuffer;
    u8 *addPrimBase;
    s32 prevOffset;
    /* This pin is load-bearing: removing it changes .text. */
    register u8 *storeBaseV1 asm("$3");
    u8 *storeBaseV0;

    initBuffers = g_TileStripBuffers;
    firstBuffer = g_TileStripStorage;
    initBuffers[0] = firstBuffer;
    g_TileStripBuffers[1] = firstBuffer + 12000;
    func_800658FC(0);

    color = 0x20;
    buffers = initBuffers;
    bufferIndex = 0;
    do {
        row = 0;
        y = 0x5A;
        do {
            col = 0;
            yStart = y;
            xStep = 0;
            do {
                linear = (row << 5) + col;
                buffer = buffers[0];
                offset = linear << 4;
                func_80064FF8((u8 *)((s32)offset + (s32)buffer));
                storeBaseV1 = buffers[0];
                *(s16 *)((s32)offset + (s32)storeBaseV1 + 0xC) = 2;
                storeBaseV1 = buffers[0];
                *(s16 *)((s32)offset + (s32)storeBaseV1 + 0xE) = 1;
                storeBaseV1 = buffers[0];
                *(s16 *)((s32)offset + (s32)storeBaseV1 + 8) = 0xCD - xStep;
                storeBaseV0 = buffers[0];
                *(s16 *)((s32)offset + (s32)storeBaseV0 + 0xA) = yStart;
                buffers[0][offset + 4] = color;
                buffers[0][offset + 5] = color;
                buffers[0][offset + 6] = color;

                if (linear > 0) {
                    addPrimBase = buffers[0];
                    prevOffset = offset - 0x10;
                    AddPrim((void *)(prevOffset + (s32)addPrimBase), (void *)((s32)offset + (s32)addPrimBase));
                }

                col++;
                xStep += 3;
            } while (col < 0x20);

            row++;
            y += 2;
        } while (row < 0x10);

        bufferIndex = bufferIndex + 1;
        buffers++;
    } while (bufferIndex < 2);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/race/GameDrawTimeValue", func_8003425C);

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/race/GameDrawTimeValue", func_8003479C);

#include "common.h"
#include "game/vector.h"
#include "game/race.h"
#include "game/render.h"
#include "game/screens.h"
#include "game/state.h"
#include "psyq/gpu.h"

/* The strip buffers hold back-to-back 0x10-byte TILEs; func_80064FF8 is
 * SetTile. The retail code reloads the buffer base before each field store,
 * so the base is passed in rather than held in a pointer. */
#define TILE_AT(base, byteOffset) (*(TILE *)((s32)(byteOffset) + (s32)(base)))

/* Static text buffers the two time drawers format into, seeded in .data with
 * "0'00\"000" and "00'00\"". The named split symbols are the digit positions
 * each writer touches; the separators are never rewritten. */
extern u8 g_TimeTextBuffer[] asm("D_8007DF04");

void func_80016754(s32 x, s32 y, void *str, s32 color);

void DrawTimeValue(s32 x, s32 y, s32 value, s32 color, s32 divisor);
void DrawTimeValue(s32 x, s32 y, s32 value, s32 color, s32 divisor) {
    s32 savedX;
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

void DrawMinuteSecondTime(s32 x, s32 y, s32 ticks, s32 color);
void DrawMinuteSecondTime(s32 x, s32 y, s32 ticks, s32 color) {
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
extern s32 g_FrameParity;

void func_80066604(void *packet, void *rect);
void AddPrim(void *ot, void *prim);

void *QueueDrawAreaPrim(void *ot, void *packet, s16 x, s16 y, s32 w, s32 h);
void *QueueDrawAreaPrim(void *ot, void *packet, s16 x, s16 y, s32 w, s32 h) {
    void *oldPacket;
    s16 rect[4];
    s32 offset;

    offset = (g_FrameParity * 15) << 4;
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

/* The two 12000-byte TILE strips BuildTileStrips builds, backed by
 * g_TileStripStorage. Nothing in the decompiled tree reads them back, so what
 * the strip renders is still unestablished -- see docs/names.md 15g. */
extern u8 *g_TileStripBuffers[2];
extern u8 g_TileStripStorage[];

void func_800658FC(s32 mode);
void func_80064FF8(u8 *prim);

void BuildTileStrips(void);
void BuildTileStrips(void) {
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
                linear = (row * 32) + col;
                buffer = buffers[0];
                offset = linear * 16;
                func_80064FF8((u8 *)((u8 *)offset + (s32)buffer));
                storeBaseV1 = buffers[0];
                TILE_AT(storeBaseV1, offset).w = 2;
                storeBaseV1 = buffers[0];
                TILE_AT(storeBaseV1, offset).h = 1;
                storeBaseV1 = buffers[0];
                TILE_AT(storeBaseV1, offset).x0 = 0xCD - xStep;
                storeBaseV0 = buffers[0];
                TILE_AT(storeBaseV0, offset).y0 = yStart;
                TILE_AT(buffers[0], offset).t.r0 = color;
                TILE_AT(buffers[0], offset).t.g0 = color;
                TILE_AT(buffers[0], offset).t.b0 = color;

                if (linear > 0) {
                    addPrimBase = buffers[0];
                    prevOffset = offset - 0x10;
                    AddPrim((void *)(prevOffset + (s32)addPrimBase), (void *)((u8 *)offset + (s32)addPrimBase));
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

extern s32 g_RacePaused;
extern s32 g_CountdownBoardOffset asm("D_8007DF18");
extern u32 g_CountdownDigitPatterns[16] asm("D_8007DEC0");
extern CVec g_CountdownCellColors[] asm("D_8007DF1C");

s32 GameQueueDrawModePrimWide(
    s32 ot, s32 prim, s32 tpage) asm("func_80017390");
void AddPrimsWide(void *ot, void *first, void *last) asm("func_80064E18");

void DrawStartCountdown(s32 sceneTimer) {
    s32 timer;
    s32 phase;
    s32 halfStep;
    s32 wipeStart;
    s32 row;
    s32 column;
    u32 pattern;
    s32 colorBank;
    s32 phaseIsNegative;
    s32 rowOffset;
    u32 *firstPattern;
    u32 *patternBeforeFirst;
    u32 *phasePattern;
    TILE *tiles;
    s32 cursor;
    u8 *packet;
    s32 rangeTimer;
    u8 *orderingTable;
    SPRT *sprite;
    u8 *backdrop;

    timer = sceneTimer;
    orderingTable = g_DrawBuffer + 0xD0;
    if (timer < 105) {
        return;
    }
    rangeTimer = timer - 90;
    if (rangeTimer >= 210) {
        return;
    }

    phase = rangeTimer / 30;
    if (phase < 0) {
        phase = 0;
    } else if (phase >= 5) {
        phase = -1;
    }

    halfStep = (timer % 30) / 2;

    if (phase == 4 || phase < 0) {
        halfStep = (timer & 2) << 2;
    } else if (phase == 0) {
        halfStep = 0;
    } else {
        if (halfStep >= 8) {
            halfStep = 8;
        } else if (halfStep <= 0) {
            halfStep = 0;
        }
    }

    row = 0;
    phaseIsNegative = phase < row;
    wipeStart = 7 - halfStep;
    tiles = (TILE *)g_TileStripBuffers[g_FrameParity];

    do {
        firstPattern = g_CountdownDigitPatterns;
        patternBeforeFirst = firstPattern - 64;
        phasePattern = patternBeforeFirst + (phase * 16);
        if (phase == 0) {
            pattern = -1;
        } else if (phaseIsNegative) {
            pattern = firstPattern[row];
        } else {
            pattern = phasePattern[row];
        }
        column = 0;
        if (wipeStart < row) {
            if (row < halfStep + 8) {
                pattern = ~pattern;
            }
        }
        rowOffset = row * 32;
        do {
            u8 *color =
                (u8 *)(((rowOffset + column) << 4) + (s32)tiles) + 4;
            colorBank = 0;
            if (phase == 4 || phaseIsNegative) {
                colorBank = 1;
            }
            {
                CVec *colors =
                    (CVec *)(
                        (u8 *)g_CountdownCellColors + (colorBank * 8));
                *(CVec *)color = colors[pattern % 2];
            }
            pattern >>= 1;
            column++;
        } while (column < 32);
        row++;
    } while (row < 16);

    if (phase < 0) {
        g_CountdownBoardOffset -= 16;
        if (g_CountdownBoardOffset < -240) {
            g_CountdownBoardOffset = -240;
        }
    } else {
        g_CountdownBoardOffset = 0;
    }

    cursor = *(s32 *)0x1F800000;
    backdrop =
        (u8 *)GameQueueDrawModePrimWide(
            (s32)(g_DrawBuffer + 0xD0), cursor, 9);
    pattern = g_CountdownBoardOffset;
    *(u8 **)0x1F800000 = backdrop;
    cursor = (s32)GameQueueTexturePacketWide(
        orderingTable,
        GameQueueTexturePacketWide(
            orderingTable, backdrop, 0x70, pattern + 66,
            0x60, 0x18, 0xA0, 0xE8, 0x60, 0x18, 0x784E, 9,
            GAME_TEXTURE_PACKET_SPRT),
        0x70, g_CountdownBoardOffset + 122,
        0x60, 0x18, 0xA0, 0xE8, 0x60, 0x18, 0x784E, 9,
        GAME_TEXTURE_PACKET_SPRT);

    packet = (u8 *)cursor;
    sprite = (SPRT *)packet;
    for (row = 0; row < 6; row++) {
        SetSprt((void *)cursor);
        sprite->w = 0x20;
        sprite->h = 0x18;
        sprite->u0 = 0xE0;
        sprite->v0 = 0xD0;
        sprite->x0 = (row % 3) * 32 + 112;
        sprite->y0 =
            (row / 3) * 56 + ((u16)g_CountdownBoardOffset + 66);

        if ((u32)phase < 4) {
            if (phase - 1 == row % 3) {
                halfStep = timer % 30;
                if (halfStep < 16) {
                    pattern = halfStep * 8;
                } else {
                    pattern = 0x80;
                }
            } else {
                pattern = 0x80;
            }
            if (phase - 1 >= row % 3) {
                sprite->clut = 0x7851;
            } else {
                sprite->clut = 0x784F;
            }
        } else {
            if (phase == 4) {
                halfStep = timer % 30;
                if (halfStep < 10) {
                    pattern = halfStep * 12;
                } else {
                    pattern = 0x80;
                }
            } else {
                pattern = 0x80;
            }
            sprite->clut = 0x7850;
        }

        sprite->t.r0 = pattern;
        sprite->t.g0 = pattern;
        sprite->t.b0 = pattern;
        {
            SPRT *currentSprite = sprite;

            sprite++;
            packet += 20;
            cursor += 20;
            AddPrim(orderingTable, currentSprite);
        }
    }

    *(s32 *)0x1F800000 = cursor;
    cursor = GameQueueDrawModePrimWide(
        (s32)(g_DrawBuffer + 0xD0), cursor, 0xC);
    *(s32 *)0x1F800000 = cursor;

    if (phase > 0) {
        if (g_RacePaused == 0) {
            AddPrimsWide(orderingTable, tiles, (u8 *)tiles + 8176);
        }
    }

    tiles = *(TILE **)0x1F800000;
    SetTile(tiles);
    rangeTimer = (u16)g_CountdownBoardOffset + 88;
    tiles->w = 0x64;
    tiles->h = 0x24;
    tiles->x0 = 0x6E;
    tiles->t.r0 = 5;
    tiles->t.g0 = 5;
    tiles->t.b0 = 5;
    tiles->y0 = rangeTimer;
    AddPrim(orderingTable, tiles++);
    *(TILE **)0x1F800000 = tiles;
}

extern s16 g_RaceOptionScroll0 asm("D_8007DF30");
extern s16 g_RaceOptionScroll1 asm("D_8007DF32");
extern char g_RaceOptionMarquee[4][40] asm("D_8007DF34");
extern s32 g_RaceOptionPulseAngle asm("D_8007DF2C");
extern u8 *g_CourseProgress;

s32 rcos(s32 angle) asm("func_80068634");
u8 *AddTilePrim(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 r,
    s32 g,
    s32 b);

void DrawRaceOptionMenu(s32 cursorRow) {
    register s32 selectedRow = cursorRow;
    u8 *ot;
    u8 *firstNext;
    register s32 brightness;
    s32 marquee;

    ot = g_DrawBuffer + 0xCC;
    {
        register SPRT *sprite asm("$18");
        register void *drawPrim;

        sprite = (SPRT *)0x1F800000;
        sprite = *(SPRT **)sprite;
        SetSprt(sprite);
        SetShadeTex(sprite, 0);
        sprite->x0 = 0x8C;
        sprite->y0 = 0x5A;
        sprite->w = 0x28;
        sprite->h = 8;
        sprite->u0 = 0xD8;
        sprite->v0 = 0x38;
        sprite->clut = 0x7893;
        if (g_RaceOptionScroll0 & 0x10) {
            asm("" : : "r"(sprite));
            brightness = 0x80;
        } else {
            brightness = 0x40;
        }
        sprite->t.r0 = brightness;
        sprite->t.g0 = brightness;
        sprite->t.b0 = brightness;
        asm("" ::: "memory");
        drawPrim = sprite;
        sprite++;
        AddPrim(ot, drawPrim);

        g_RaceOptionScroll0 -= 4;
        g_RaceOptionScroll1 -= 4;
        if ((g_RaceOptionScroll0 >> 2) < -0x9C) {
            g_RaceOptionScroll0 = 0xF0;
        }
        if ((g_RaceOptionScroll1 >> 2) < -0x9C) {
            g_RaceOptionScroll1 = 0xF0;
        }

        firstNext =
            QueueDrawAreaPrim(ot, sprite, 0, 0, 0x140, 0xF0);
    }

    {
        register u8 *scratchPacket;
        register s32 fontU;
        register s16 scroll0;
        register char *marqueeBase asm("$16");
        register u8 *prim asm("$18");
        u8 *drawPrim;

        {
            register s32 textY = 0x8A;
            register s32 textColor = 0x7811;

            asm(
                "" : "=r"(textY), "=r"(textColor) :
                "0"(textY), "1"(textColor));
            scratchPacket = (u8 *)0x1F800000;
            scroll0 = g_RaceOptionScroll0;
            marqueeBase = &g_RaceOptionMarquee[0][0];
            *(u8 **)scratchPacket = firstNext;
            marquee = (g_SceneTimer & 3) * 40;
            func_80016754(
                (scroll0 >> 2) + 0xA0,
                textY,
                &marqueeBase[marquee],
                textColor);
        }
        {
            register s32 secondTextY = 0x8A;

            asm(
                "" : "=r"(secondTextY) :
                "0"(secondTextY));
            marqueeBase += 20;
            func_80016754(
                (g_RaceOptionScroll1 >> 2) + 0xA0,
                secondTextY,
                &marqueeBase[marquee],
                0x7811);
        }

        scratchPacket = *(u8 **)scratchPacket;
        drawPrim = QueueDrawAreaPrim(
            ot, scratchPacket, 0x72, 0x8A, 0x5C, 0xC);
        fontU = 0xD0;
        prim = GameQueueSpriteWide(
            ot, drawPrim, 0x88, 0x6A, 0x30, 8, fontU, 0x10, 0x7893);
        if (g_GrandPrixMode != 0) {
            prim = GameQueueSpriteWide(
                ot, prim, 0x88, 0x74, 0x30, 8, 0xA0, 0x28, 0x7893);
            prim = GameQueueSpriteWide(
                ot, prim, 0x84, 0x7E, 0x30, 8, fontU, 0x28, 0x7893);
            prim = GameQueueSpriteWide(
                ot,
                prim,
                0xB8,
                0x7E,
                8,
                8,
                *(s16 *)(g_CourseProgress + 6) * 8,
                0,
                0x78CC);
            prim = GameQueueSpriteWide(
                ot, prim, 0x78, 0x7E, 8, 8, 0xD8, 8, 0x78CC);
            prim = GameQueueSpriteWide(
                ot, prim, 0xC0, 0x7E, 8, 8, 0xE8, 8, 0x78CC);
        } else {
            prim = GameQueueSpriteWide(
                ot, prim, 0x85, 0x74, 0x38, 8, 0xA0, 0x40, 0x7893);
            prim = GameQueueSpriteWide(
                ot, prim, 0x90, 0x7E, 0x28, 8, 0xD8, 0x40, 0x7893);
        }

        {
            s32 y;

            y = selectedRow * 10 + 0x68;
            prim = AddTilePrim(
                ot, prim, 0x80, y, 0x40, 1, 0xFF, 0xFF, 0);
            prim = AddTilePrim(
                ot, prim, 0x80, y + 0xB, 0x40, 1, 0xFF, 0xFF, 0);
            prim = AddTilePrim(
                ot, prim, 0x80, y, 1, 0xB, 0xFF, 0xFF, 0);
            prim = AddTilePrim(
                ot, prim, 0xBF, y, 1, 0xB, 0xFF, 0xFF, 0);
        }

        prim =
            GameQueueTileTrans(ot, prim, 0x70, 0x50, 0x60, 0x48, 8, 8, 8);

        {
            POLY_FT4 *quadBase;

            quadBase = (POLY_FT4 *)GameQueueTileTrans(
                ot, prim, 0x70, 0x50, 0x60, 0x48, 8, 8, 8);
            {
                register POLY_FT4 *quad asm("$17") = quadBase;
                register s32 leftTrig;
                s16 left;

                g_RaceOptionPulseAngle += 0x20;
                SetPolyFT4(quad);
                quad->t.r0 = 0x60;
                quad->t.g0 = 0x60;
                quad->t.b0 = 0x60;
                g_RaceOptionPulseAngle &= 0xFFF;
                leftTrig = rcos(g_RaceOptionPulseAngle) * 0x2C;
                if (leftTrig < 0) {
                    leftTrig += 0xFFF;
                }
                left = 0xA0 - (leftTrig >> 12);
                quad->x2 = left;
                quad->x0 = left;
                quad->y1 = 0x58;
                quad->y0 = 0x58;
            }
            {
                register u8 *drawModePrim asm("$17");
                register POLY_FT4 *drawPrim;
                s32 rightTrig;
                s16 right;

                g_RaceOptionPulseAngle &= 0xFFF;
                rightTrig = ({
                    register s32 sample;

                    sample = rcos(g_RaceOptionPulseAngle);
                    asm volatile("" ::: "memory");
                    sample * 0x2C;
                });
                drawModePrim = (u8 *)(quadBase + 1);
                if (rightTrig < 0) {
                    rightTrig += 0xFFF;
                }
                right = (rightTrig >> 12) + 0xA0;
                drawPrim = quadBase;
                drawPrim->x3 = right;
                drawPrim->x1 = right;
                drawPrim->y3 = 0x90;
                drawPrim->y2 = 0x90;
                drawPrim->u0 = 0xA8;
                drawPrim->v0 = 0xA8;
                drawPrim->u1 = 0xFF;
                drawPrim->v1 = 0xA8;
                drawPrim->u2 = 0xA8;
                drawPrim->v2 = 0xE0;
                drawPrim->u3 = 0xFF;
                drawPrim->v3 = 0xE0;
                drawPrim->clut = 0x784B;
                drawPrim->tpage = 9;
                AddPrim(g_DrawBuffer + 0xCC, drawPrim);

                *(u8 **)0x1F800000 = QueueDrawModePrim(
                    g_DrawBuffer + 0xCC, drawModePrim, 9);
            }
        }
    }
}

#include "common.h"
#include "game/race.h"
#include "game/render.h"
#include "game/screens.h"
#include "game/state.h"
#include "psyq/gpu.h"

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

typedef struct UnalignedColor {
    u8 bytes[4];
} UnalignedColor;

extern s32 g_RacePaused asm("D_801E4BAC");
extern u32 g_CountdownPatterns[] asm("D_8007DEC0");
extern u32 g_CountdownColors[] asm("D_8007DF1C");
extern s32 g_CountdownSlideY asm("D_8007DF18");

void func_8003425C(s32 arg0) {
    register s32 timer asm("$23") = arg0;
    volatile s32 rawCallArea[6];
    volatile s32 localPad[2];
    u8 * volatile ot;
    register s32 wipe asm("$8");
    register s32 row asm("$17");
    register s32 col asm("$9");
    register s32 startRow asm("$18");
    register s32 negativePhase asm("$14");
    register s32 patternWord asm("$7");
    register s32 colorBank asm("$3");
    register s32 lamp asm("$17");
    register s32 intensity asm("$7");
    register s32 clut asm("$2");
    register u32 *pattern asm("$13");
    register u32 *offPattern asm("$12");
    register u8 *prim asm("$18");
    register u8 *drawPrim asm("$2");
    u8 *lampPrim;
    SPRT *sprite;
    TILE *tile;

    ot = g_DrawBuffer + 0xD0;
    if (timer < 105 || (timer - 90) >= 210) {
        return;
    }
    {
        register u8 *tiles asm("$22");
        register s32 rowBase asm("$11");
        register u8 *colorDst asm("$10");
        register u32 *patternBase asm("$4");
        register u32 *patternBias asm("$3");
        register s32 phaseOffset asm("$2");
        register s32 seven asm("$2");
        register u8 *colorTable asm("$15");
        register s32 phaseFour asm("$16");
        s32 phase;

        phase = (timer - 90) / 30;
        if (phase < 0) {
            phase = 0;
        } else if (phase >= 5) {
            phase = -1;
        }

        do {
            register s32 initialCycle asm("$2");
            initialCycle = timer % 30;
            wipe = initialCycle / 2;
        } while (0);

        asm volatile(
            ".word 0x34020004\n\t"
            ".word 0x12A20003\n\t"
            ".word 0x00008821\n\t"
            ".word 0x06A10004\n\t"
            ".word 0x00000000\n\t"
            ".word 0x32E20002\n\t"
            ".word 0x0800D0DC\n\t"
            ".word 0x00024080\n\t"
            ".word 0x12A00007\n\t"
            ".word 0x29020008\n\t"
            ".word 0x14400003\n\t"
            ".word 0x00000000\n\t"
            ".word 0x0800D0DB\n\t"
            ".word 0x34080008\n\t"
            ".word 0x1D000003\n\t"
            ".word 0x00008821\n\t"
            ".word 0x00004021\n\t"
            ".word 0x00008821"
            : "=r"(row), "=r"(wipe)
            : "r"(phase), "r"(timer), "1"(wipe));

        asm("slti %0,%1,0" : "=r"(negativePhase) : "r"(phase));
        seven = 7;
        asm("" : "=r"(seven) : "0"(seven));
        startRow = seven - wipe;
        phaseFour = 4;
        asm("" : "=r"(phaseFour) : "0"(phaseFour));
        colorTable = (u8 *)g_CountdownColors;
        asm("" : "=r"(colorTable) : "0"(colorTable));
        patternBase = g_CountdownPatterns;
        asm("" : "=r"(patternBase) : "0"(patternBase));
        patternBias = patternBase - 64;
        asm("" : "=r"(patternBias) : "0"(patternBias));
        phaseOffset = phase << 6;
        asm("" : "=r"(phaseOffset) : "0"(phaseOffset));
        pattern = (u32 *)(phaseOffset + (s32)patternBias);
        offPattern = patternBase;
        tiles = g_TileStripBuffers[g_FrameParity];
        do {
            patternWord = -1;
            if (phase != 0) {
                if (negativePhase) {
                    patternWord = *offPattern;
                } else {
                    patternWord = *pattern;
                }
            }
            col = 0;
            do {
                if (startRow < row) {
                    rowBase = row << 5;
                    if (row < wipe + 8) {
                        patternWord = ~patternWord;
                        goto row_base_ready;
                    }
                } else {
row_base_ready:
                    rowBase = row << 5;
                }
            } while (0);
            do {
                colorBank = 0;
                do {
                    register u8 *tileCell asm("$2");
                    tileCell = (u8 *)(((rowBase + col) << 4) + (s32)tiles);
                    colorDst = tileCell + 4;
                } while (0);
                if (phase == phaseFour || negativePhase) {
                    colorBank = 1;
                }
                do {
                    register s32 patternBit asm("$2");
                    colorBank <<= 3;
                    colorBank += (s32)colorTable;
                    patternBit = patternWord & 1;
                    patternBit <<= 2;
                    patternBit += colorBank;
                    *(UnalignedColor *)colorDst = *(UnalignedColor *)patternBit;
                } while (0);
                col++;
                patternWord = (u32)patternWord >> 1;
            } while (col < 32);
            pattern++;
            offPattern++;
            row++;
        } while (row < 16);

        if (phase < 0) {
            g_CountdownSlideY -= 16;
            if (g_CountdownSlideY < -240) {
                g_CountdownSlideY = -240;
            }
        } else {
            g_CountdownSlideY = 0;
        }

        {
            register s32 drawModeNine asm("$6");
            register u8 **firstScratch asm("$16");
            drawModeNine = 9;
            asm("" : : "r"(drawModeNine));
            firstScratch = (u8 **)0x1F800000;
            drawPrim = *firstScratch;
            drawPrim = GameQueueDrawModePrim(g_DrawBuffer + 0xD0, drawPrim, drawModeNine);
            asm("" : : "r"(firstScratch));
        }

        asm volatile(
            ".word 0x00402821\n\t"
            ".word 0x34060070\n\t"
            ".word 0x34140060\n\t"
            ".word 0x34130018\n\t"
            ".word 0x341200A0\n\t"
            ".word 0x8FA40030\n\t"
            ".word 0x3C078008\n\t"
            ".word 0x8CE7DF18\n\t"
            ".word 0x341100E8\n\t"
            ".word 0xAE050000\n\t"
            ".word 0x3410784E\n\t"
            ".word 0xAFB40010\n\t"
            ".word 0xAFB30014\n\t"
            ".word 0xAFB20018\n\t"
            ".word 0xAFB1001C\n\t"
            ".word 0xAFB00020\n\t"
            ".word 0x0C005BB1\n\t"
            ".word 0x24E70042\n\t"
            ".word 0x00402821\n\t"
            ".word 0x8FA40030\n\t"
            ".word 0x3C078008\n\t"
            ".word 0x8CE7DF18\n\t"
            ".word 0x34060070\n\t"
            ".word 0xAFB40010\n\t"
            ".word 0xAFB30014\n\t"
            ".word 0xAFB20018\n\t"
            ".word 0xAFB1001C\n\t"
            ".word 0xAFB00020\n\t"
            ".word 0x0C005BB1\n\t"
            ".word 0x24E7007A"
            : "=r"(drawPrim)
            : "0"(drawPrim)
            : "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10",
              "$11", "$12", "$13", "$14", "$15", "$24", "$25",
              "hi", "lo", "memory");

        {
            register s32 lampColumn asm("$20");
            register s32 partialTimerQuotient asm("$19");
            register s32 lampCycle asm("$8");
            register s32 timerSign asm("$2");
            register s32 lampRemainder asm("$4");
            s32 fpLive;

            asm volatile(
                ".word 0x3C038888\n\t"
                ".word 0x34638889\n\t"
                ".word 0x02E30018\n\t"
                ".word 0x00008821"
                : "=r"(lamp)
                : "r"(drawPrim), "r"(phase), "r"(timer));
            fpLive = 0x55555556;
            asm volatile(
                ".word 0x26B4FFFF\n\t"
                ".word 0x00409021\n\t"
                ".word 0x02408021\n\t"
                ".word 0x00001010\n\t"
                ".word 0x00571021\n\t"
                ".word 0x00029903"
                : "=r"(lampColumn), "=r"(prim), "=r"(sprite),
                  "=r"(partialTimerQuotient)
                : "r"(drawPrim), "r"(phase), "r"(timer), "r"(lamp),
                  "r"(fpLive));
            do {
                SetSprt((SPRT *)prim);
                asm volatile(
                    ".word 0x023E0018\n\t"
                    ".word 0x34020020\n\t"
                    ".word 0xA6020010\n\t"
                    ".word 0x34020018\n\t"
                    ".word 0xA6020012\n\t"
                    ".word 0x340200E0\n\t"
                    ".word 0xA202000C\n\t"
                    ".word 0x340200D0\n\t"
                    ".word 0xA202000D\n\t"
                    ".word 0x001117C3\n\t"
                    ".word 0x00001810\n\t"
                    ".word 0x00621823\n\t"
                    ".word 0x00031040\n\t"
                    ".word 0x00431021\n\t"
                    ".word 0x02222023\n\t"
                    ".word 0x00041140\n\t"
                    ".word 0x24420070\n\t"
                    ".word 0xA6020008\n\t"
                    ".word 0x000310C0\n\t"
                    ".word 0x00431023\n\t"
                    ".word 0x3C038008\n\t"
                    ".word 0x9463DF18\n\t"
                    ".word 0x000210C0\n\t"
                    ".word 0x24630042\n\t"
                    ".word 0x00431021\n\t"
                    ".word 0xA602000A"
                    : "=r"(lampRemainder)
                    : "r"(lamp), "r"(sprite), "r"(fpLive));
                if ((u32)phase < 4) {
                    timerSign = timer >> 31;
                    if (lampColumn == lampRemainder) {
                        asm volatile(
                            "subu %0,$19,$2\n\t"
                            "sll $2,%0,4\n\t"
                            "subu $2,$2,%0\n\t"
                            "sll $2,$2,1\n\t"
                            "subu %0,$23,$2"
                            : "=r"(lampCycle)
                            : "r"(timer), "r"(partialTimerQuotient),
                              "r"(timerSign));
                        intensity = 0x80;
                        if (lampCycle < 16) {
                            intensity = lampCycle * 8;
                        }
                    } else {
                        intensity = 0x80;
                        asm volatile("mult $17,$30");
                    }
                    asm volatile(
                        "sra $2,$17,31\n\t"
                        "mfhi $3\n\t"
                        "subu $3,$3,$2\n\t"
                        "sll $2,$3,1\n\t"
                        "addu $2,$2,$3\n\t"
                        "subu %0,$17,$2\n\t"
                        "slt %0,$20,%0"
                        : "=r"(clut)
                        : "r"(lamp));
                    if (clut) {
                        clut = 0x784F;
                        goto clut_ready;
                    }
                    clut = 0x7851;
                    goto clut_ready;
                } else {
                    intensity = 0x80;
                    if (phase == 4) {
                        timerSign = timer >> 31;
                        asm volatile(
                            "subu %0,$19,$2\n\t"
                            "sll $2,%0,4\n\t"
                            "subu $2,$2,%0\n\t"
                            "sll $2,$2,1\n\t"
                            "subu %0,$23,$2"
                            : "=r"(lampCycle)
                            : "r"(timer), "r"(partialTimerQuotient),
                              "r"(timerSign));
                        if (lampCycle < 10) {
                            timerSign = lampCycle << 1;
                            timerSign += lampCycle;
                            intensity = timerSign << 2;
                        }
                    }
                    asm volatile("");
                    clut = 0x7850;
                }
clut_ready:
                sprite->clut = clut;
                asm("" : : : "memory");
                lampPrim = (u8 *)sprite;
                asm("" : "=r"(sprite) : "0"(sprite));
                sprite->t.r0 = intensity;
                sprite->t.g0 = intensity;
                sprite->t.b0 = intensity;
                asm("" : : : "memory");
                sprite = (SPRT *)((u8 *)sprite + sizeof(SPRT));
                prim += sizeof(SPRT);
                AddPrim(ot, lampPrim);
                lamp++;
            } while (lamp < 6);
        }

        *(u8 **)0x1F800000 = prim;
        prim = GameQueueDrawModePrim(g_DrawBuffer + 0xD0, prim, 12);
        *(u8 **)0x1F800000 = prim;
        if (phase > 0 && g_RacePaused == 0) {
            AddPrims((u_long *)ot, (u_long)tiles, (u_long *)(tiles + 0x1FF0));
        }

        tiles = *(u8 **)0x1F800000;
        SetTile((TILE *)tiles);
        {
            register u8 *finalPrim asm("$5");
            register u8 *finalOt asm("$4");
            register s32 finalY asm("$3");
            finalPrim = tiles;
            finalOt = ot;
            finalY = (u16)g_CountdownSlideY;
            asm("" : : "r"(finalPrim), "r"(finalOt), "r"(finalY));
            ((TILE *)tiles)->w = 0x64;
            asm("" : : : "memory");
            ((TILE *)tiles)->h = 0x24;
            asm("" : : : "memory");
            ((TILE *)tiles)->x0 = 0x6E;
            asm("" : : : "memory");
            ((TILE *)tiles)->t.r0 = 5;
            ((TILE *)tiles)->t.g0 = 5;
            ((TILE *)tiles)->t.b0 = 5;
            asm("" : : : "memory");
            asm volatile(
                ".word 0x24630058\n\t"
                ".word 0xA6C3000A"
                : "=r"(finalY)
                : "0"(finalY), "r"(tiles)
                : "memory");
            asm volatile(
                ".word 0x0C019377\n\t"
                ".word 0x26D60010"
                : "=r"(tiles)
                : "0"(tiles), "r"(finalPrim), "r"(finalOt), "r"(finalY)
                : "$31", "memory");
            *(u8 **)0x1F800000 = tiles;
        }
    }
}

extern s16 g_RaceOptionScroll0 asm("D_8007DF30");
extern s16 g_RaceOptionScroll1 asm("D_8007DF32");
extern char g_RaceOptionMarquee[4][40] asm("D_8007DF34");
extern s32 g_RaceOptionPulseAngle asm("D_8007DF2C");
extern u8 *g_CourseProgress asm("D_8009E67C");

s32 func_80068634(s32 angle);
u8 *QueueSpriteWide(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) asm("func_80016EC4");
u8 *GameAddTilePrim(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 r,
    s32 g,
    s32 b) asm("func_80032F34");

void GameDrawRaceOptionMenu(s32 cursorRow) {
    register s32 selectedRow asm("$23") = cursorRow;
    u8 *ot;
    u8 *firstNext;
    register s32 brightness asm("$2");
    s32 marquee;

    ot = g_DrawBuffer + 0xCC;
    {
        register SPRT *sprite asm("$18");
        register void *drawPrim asm("$5");

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
            GameQueueDrawAreaPrim(ot, sprite, 0, 0, 0x140, 0xF0);
    }

    {
        register u8 *scratchPacket asm("$18");
        register s32 fontU asm("$22");
        register s16 scroll0 asm("$4");
        register char *marqueeBase asm("$16");
        register u8 *prim asm("$18");
        u8 *drawPrim;

        {
            register s32 textY asm("$5") = 0x8A;
            register s32 textColor asm("$7") = 0x7811;

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
            register s32 secondTextY asm("$5") = 0x8A;

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
        drawPrim = GameQueueDrawAreaPrim(
            ot, scratchPacket, 0x72, 0x8A, 0x5C, 0xC);
        fontU = 0xD0;
        prim = QueueSpriteWide(
            ot, drawPrim, 0x88, 0x6A, 0x30, 8, fontU, 0x10, 0x7893);
        if (g_GrandPrixMode != 0) {
            prim = QueueSpriteWide(
                ot, prim, 0x88, 0x74, 0x30, 8, 0xA0, 0x28, 0x7893);
            prim = QueueSpriteWide(
                ot, prim, 0x84, 0x7E, 0x30, 8, fontU, 0x28, 0x7893);
            prim = QueueSpriteWide(
                ot,
                prim,
                0xB8,
                0x7E,
                8,
                8,
                *(s16 *)(g_CourseProgress + 6) * 8,
                0,
                0x78CC);
            prim = QueueSpriteWide(
                ot, prim, 0x78, 0x7E, 8, 8, 0xD8, 8, 0x78CC);
            prim = QueueSpriteWide(
                ot, prim, 0xC0, 0x7E, 8, 8, 0xE8, 8, 0x78CC);
        } else {
            prim = QueueSpriteWide(
                ot, prim, 0x85, 0x74, 0x38, 8, 0xA0, 0x40, 0x7893);
            prim = QueueSpriteWide(
                ot, prim, 0x90, 0x7E, 0x28, 8, 0xD8, 0x40, 0x7893);
        }

        {
            s32 y;

            y = selectedRow * 10 + 0x68;
            prim = GameAddTilePrim(
                ot, prim, 0x80, y, 0x40, 1, 0xFF, 0xFF, 0);
            prim = GameAddTilePrim(
                ot, prim, 0x80, y + 0xB, 0x40, 1, 0xFF, 0xFF, 0);
            prim = GameAddTilePrim(
                ot, prim, 0x80, y, 1, 0xB, 0xFF, 0xFF, 0);
            prim = GameAddTilePrim(
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
                register s32 leftTrig asm("$3");
                s16 left;

                g_RaceOptionPulseAngle += 0x20;
                SetPolyFT4(quad);
                quad->t.r0 = 0x60;
                quad->t.g0 = 0x60;
                quad->t.b0 = 0x60;
                g_RaceOptionPulseAngle &= 0xFFF;
                leftTrig = func_80068634(g_RaceOptionPulseAngle) * 0x2C;
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
                register POLY_FT4 *drawPrim asm("$5");
                s32 rightTrig;
                s16 right;

                g_RaceOptionPulseAngle &= 0xFFF;
                rightTrig = ({
                    register s32 sample asm("$2");

                    sample = func_80068634(g_RaceOptionPulseAngle);
                    asm volatile("" ::: "memory");
                    sample * 0x2C;
                });
                drawModePrim = (u8 *)(quadBase + 1);
                if (rightTrig < 0) {
                    rightTrig += 0xFFF;
                }
                right = (rightTrig >> 12) + 0xA0;
                asm("" : "=r"(right) : "0"(right));
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

                *(u8 **)0x1F800000 = GameQueueDrawModePrim(
                    g_DrawBuffer + 0xCC, drawModePrim, 9);
            }
        }
    }
}

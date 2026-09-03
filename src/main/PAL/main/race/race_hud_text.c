#include "common.h"
#include "game/prim.h"
#include "game/race.h"
#include "game/save_internal.h"
#include "game/fmv_internal.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/screens.h"
#include "game/state.h"
#include "game/vector.h"
#include "psyq/gpu.h"


/* The strip buffers hold back-to-back 0x10-byte TILEs; SetTile is
 * SetTile. The retail code reloads the buffer base before each field store,
 * so the base is passed in rather than held in a pointer. */
typedef union TileStripAddress {
    s32 byteOffset;
    s32 value;
    u8 *bytes;
    TILE *tile;
} TileStripAddress;

static __inline__ TILE *GetTileAtByteOffset(u8 *base, s32 byteOffset) {
    TileStripAddress address;

    address.bytes = base;
    address.value = byteOffset + address.value;
    return address.tile;
}

/* Which of the two frame buffers is being drawn, 0 or 1; the main loop sets
 * it beside g_DrawBuffer. Here it turns into the 240-line y bias of the
 * drawing-area rect. */

u8 *QueueDrawAreaPrim(void *ot, DrawPacket *packet, s16 x, s16 y, s32 w, s32 h) {
    DrawPacket *oldPacket;
    RenderBufferAddress nextPacket;
    Rect rect;
    s32 offset;

    offset = (g_FrameParity * 15) << 4;
    rect.x = x;
    rect.y = y + offset;
    rect.w = w;
    rect.h = h;
    SetDrawArea(packet, &rect);
    oldPacket = packet;
    packet++;
    AddPrim(ot, oldPacket);
    nextPacket.drawPacket = packet;
    return nextPacket.bytes;
}

void BuildTileStrips(void) {
    RenderBufferAddress *initBuffers;
    RenderBufferAddress *buffers;
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
    initBuffers[0].bytes = firstBuffer;
    g_TileStripBuffers[1].bytes = firstBuffer + 12000;
    DrawSync(0);

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
                buffer = buffers[0].bytes;
                offset = linear * 16;
                SetTile(GetTileAtByteOffset(buffer, offset));
                storeBaseV1 = buffers[0].bytes;
                GetTileAtByteOffset(storeBaseV1, offset)->w = 2;
                storeBaseV1 = buffers[0].bytes;
                GetTileAtByteOffset(storeBaseV1, offset)->h = 1;
                storeBaseV1 = buffers[0].bytes;
                GetTileAtByteOffset(storeBaseV1, offset)->x0 = 0xCD - xStep;
                storeBaseV0 = buffers[0].bytes;
                GetTileAtByteOffset(storeBaseV0, offset)->y0 = yStart;
                GetTileAtByteOffset(buffers[0].bytes, offset)->t.r0 = color;
                GetTileAtByteOffset(buffers[0].bytes, offset)->t.g0 = color;
                GetTileAtByteOffset(buffers[0].bytes, offset)->t.b0 = color;

                if (linear > 0) {
                    addPrimBase = buffers[0].bytes;
                    prevOffset = offset - 0x10;
                    AddPrim(GetTileAtByteOffset(addPrimBase, prevOffset),
                            GetTileAtByteOffset(addPrimBase, offset));
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

void DrawStartCountdown(s32 sceneTimer) {
    s32 timer;
    SignedWord phase;
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
    u8 *cursor;
    u8 *packet;
    s32 rangeTimer;
    u8 *orderingTable;
    SPRT *sprite;
    u8 *backdrop;
    RenderBufferAddress packetAddress;

    timer = sceneTimer;
    orderingTable = g_DrawBuffer + 0xD0;
    if (timer < 105) {
        return;
    }
    rangeTimer = timer - 90;
    if (rangeTimer >= 210) {
        return;
    }

    phase.value = rangeTimer / 30;
    if (phase.value < 0) {
        phase.value = 0;
    } else if (phase.value >= 5) {
        phase.value = -1;
    }

    halfStep = (timer % 30) / 2;

    if (phase.value == 4 || phase.value < 0) {
        halfStep = (timer & 2) << 2;
    } else if (phase.value == 0) {
        halfStep = 0;
    } else {
        if (halfStep >= 8) {
            halfStep = 8;
        } else if (halfStep <= 0) {
            halfStep = 0;
        }
    }

    row = 0;
    phaseIsNegative = phase.value < row;
    wipeStart = 7 - halfStep;
    tiles = g_TileStripBuffers[g_FrameParity].tile;

    do {
        firstPattern = g_CountdownDigitPatterns;
        patternBeforeFirst = firstPattern - 64;
        phasePattern = patternBeforeFirst + (phase.value * 16);
        if (phase.value == 0) {
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
            RenderBufferAddress tileBase;
            RenderBufferAddress color;

            tileBase.pointer = tiles;
            color.value = ((rowOffset + column) << 4) +
                tileBase.value + sizeof(u32);
            colorBank = 0;
            if (phase.value == 4 || phaseIsNegative) {
                colorBank = 1;
            }
            {
                CVec *colors = &g_CountdownCellColors[colorBank * 2];
                *color.color = colors[pattern & 1];
            }
            pattern >>= 1;
            column++;
        } while (column < 32);
        row++;
    } while (row < 16);

    if (phase.value < 0) {
        g_CountdownBoardOffset -= 16;
        if (g_CountdownBoardOffset < -240) {
            g_CountdownBoardOffset = -240;
        }
    } else {
        g_CountdownBoardOffset = 0;
    }

    cursor = SCRATCH_PRIM_CURSOR_AS(u8);
    backdrop = QueueDrawModePrim(
        g_DrawBuffer + 0xD0, cursor, 9);
    pattern = g_CountdownBoardOffset;
    SCRATCH_PRIM_CURSOR_AS(u8) = backdrop;
    cursor = GameQueueTexturePacketWide(
        orderingTable,
        GameQueueTexturePacketWide(
            orderingTable, backdrop, 0x70, pattern + 66,
            0x60, 0x18, 0xA0, 0xE8, 0x60, 0x18, 0x784E, 9,
            GAME_TEXTURE_PACKET_SPRT),
        0x70, g_CountdownBoardOffset + 122,
        0x60, 0x18, 0xA0, 0xE8, 0x60, 0x18, 0x784E, 9,
        GAME_TEXTURE_PACKET_SPRT);

    packet = cursor;
    packetAddress.bytes = packet;
    sprite = packetAddress.sprite;
    for (row = 0; row < 6; row++) {
        SetSprt(cursor);
        sprite->w = 0x20;
        sprite->h = 0x18;
        sprite->u0 = 0xE0;
        sprite->v0 = 0xD0;
        sprite->x0 = (row % 3) * 32 + 112;
        sprite->y0 =
            (row / 3) * 56 + ((u16)g_CountdownBoardOffset + 66);

        if (phase.unsignedValue < 4) {
            if (phase.value - 1 == row % 3) {
                halfStep = timer % 30;
                if (halfStep < 16) {
                    pattern = halfStep * 8;
                } else {
                    pattern = 0x80;
                }
            } else {
                pattern = 0x80;
            }
            if (phase.value - 1 >= row % 3) {
                sprite->clut = 0x7851;
            } else {
                sprite->clut = 0x784F;
            }
        } else {
            if (phase.value == 4) {
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

    SCRATCH_PRIM_CURSOR_AS(u8) = cursor;
    cursor = QueueDrawModePrim(g_DrawBuffer + 0xD0, cursor, 0xC);
    SCRATCH_PRIM_CURSOR_AS(u8) = cursor;

    if (phase.value > 0) {
        if (g_RacePaused == 0) {
            AddPrims(orderingTable, tiles, tiles + 511);
        }
    }

    tiles = SCRATCH_PRIM_CURSOR_AS(TILE);
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
    SCRATCH_PRIM_CURSOR_AS(TILE) = tiles;
}

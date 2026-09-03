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


void DrawRaceOptionMenu(s32 cursorRow) {
    register s32 selectedRow = cursorRow;
    u8 *ot;
    u8 *firstNext;
    register s32 brightness;
    s32 marquee;
    register RenderBufferAddress prim asm("$18");

    ot = g_DrawBuffer + 0xCC;
    {
        register void *drawPrim;

        prim.bytes = SCRATCH_PRIM_CURSOR_AS(u8);
        SetSprt(prim.sprite);
        SetShadeTex(prim.sprite, 0);
        prim.sprite->x0 = 0x8C;
        prim.sprite->y0 = 0x5A;
        prim.sprite->w = 0x28;
        prim.sprite->h = 8;
        prim.sprite->u0 = 0xD8;
        prim.sprite->v0 = 0x38;
        prim.sprite->clut = 0x7893;
        if (g_RaceOptionScroll0 & 0x10) {
            asm("" : : "r"(prim.bytes));
            brightness = 0x80;
        } else {
            brightness = 0x40;
        }
        prim.sprite->t.r0 = brightness;
        prim.sprite->t.g0 = brightness;
        prim.sprite->t.b0 = brightness;
        asm("" ::: "memory");
        drawPrim = prim.pointer;
        prim.bytes += sizeof(SPRT);
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
            QueueDrawAreaPrim(ot, prim.drawPacket, 0, 0, 0x140, 0xF0);
    }

    {
        register RenderBufferAddress scratchPacket;
        register s32 fontU;
        register s16 scroll0;
        register char *marqueeBase asm("$16");
        u8 *drawPrim;

        {
            register s32 textY = 0x8A;
            register s32 textColor = 0x7811;

            asm(
                "" : "=r"(textY), "=r"(textColor) :
                "0"(textY), "1"(textColor));
            scratchPacket.bytes = SCRATCHPAD_BYTES;
            scroll0 = g_RaceOptionScroll0;
            marqueeBase = &g_RaceOptionMarquee[0][0];
            *scratchPacket.packetLink = firstNext;
            marquee = (g_SceneTimer & 3) * (20 * 2);
            DrawText8x8(
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
            DrawText8x8(
                (g_RaceOptionScroll1 >> 2) + 0xA0,
                secondTextY,
                &marqueeBase[marquee],
                0x7811);
        }

        scratchPacket.bytes = *scratchPacket.packetLink;
        drawPrim = QueueDrawAreaPrim(
            ot, scratchPacket.drawPacket, 0x72, 0x8A, 0x5C, 0xC);
        fontU = 0xD0;
        prim.bytes = GameQueueSprite(
            ot, drawPrim, 0x88, 0x6A, 0x30, 8, fontU, 0x10, 0x7893);
        if (g_GrandPrixMode != 0) {
            prim.bytes = GameQueueSprite(
                ot, prim.bytes, 0x88, 0x74, 0x30, 8, 0xA0, 0x28, 0x7893);
            prim.bytes = GameQueueSprite(
                ot, prim.bytes, 0x84, 0x7E, 0x30, 8, fontU, 0x28, 0x7893);
            prim.bytes = GameQueueSprite(
                ot,
                prim.bytes,
                0xB8,
                0x7E,
                8,
                8,
                g_CourseProgress->retriesRemaining * 8,
                0,
                0x78CC);
            prim.bytes = GameQueueSprite(
                ot, prim.bytes, 0x78, 0x7E, 8, 8, 0xD8, 8, 0x78CC);
            prim.bytes = GameQueueSprite(
                ot, prim.bytes, 0xC0, 0x7E, 8, 8, 0xE8, 8, 0x78CC);
        } else {
            prim.bytes = GameQueueSprite(
                ot, prim.bytes, 0x85, 0x74, 0x38, 8, 0xA0, 0x40, 0x7893);
            prim.bytes = GameQueueSprite(
                ot, prim.bytes, 0x90, 0x7E, 0x28, 8, 0xD8, 0x40, 0x7893);
        }

        {
            s32 y;

            y = selectedRow * 10 + 0x68;
            prim.bytes = AddTilePrim(
                ot, prim.bytes, 0x80, y, 0x40, 1, 0xFF, 0xFF, 0);
            prim.bytes = AddTilePrim(
                ot, prim.bytes, 0x80, y + 0xB, 0x40, 1, 0xFF, 0xFF, 0);
            prim.bytes = AddTilePrim(
                ot, prim.bytes, 0x80, y, 1, 0xB, 0xFF, 0xFF, 0);
            prim.bytes = AddTilePrim(
                ot, prim.bytes, 0xBF, y, 1, 0xB, 0xFF, 0xFF, 0);
        }

        prim.bytes =
            GameQueueTileTrans(ot, prim.bytes, 0x70, 0x50, 0x60, 0x48, 8, 8, 8);

        {
            POLY_FT4 *quadBase;
            register POLY_FT4 *quad asm("$17");
            RenderBufferAddress quadAddress;

            quadAddress.bytes = GameQueueTileTrans(
                ot, prim.bytes, 0x70, 0x50, 0x60, 0x48, 8, 8, 8);
            quadBase = quadAddress.polyFT4;
            {
                register s32 leftTrig;
                s16 left;

                quad = quadBase;
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
                register POLY_FT4 *drawPrim;
                s32 rightTrig;
                register s32 sample;
                s16 right;
                RenderBufferAddress cursor;

                g_RaceOptionPulseAngle &= 0xFFF;
                sample = rcos(g_RaceOptionPulseAngle);
                asm volatile("" ::);
                rightTrig = sample * 0x2C;
                quad = quadBase + 1;
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

                cursor.polyFT4 = quad;
                SCRATCH_PRIM_CURSOR_AS(u8) = QueueDrawModePrim(
                    g_DrawBuffer + 0xCC, cursor.bytes, 9);
            }
        }
    }
}

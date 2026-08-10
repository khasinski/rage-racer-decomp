#include "common.h"
#include "game/prim.h"
#include "game/car.h"
#include "game/race.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/track.h"
#include "psyq/gpu.h"

/* The GPU packet cursor: scratchpad word 0. Every emitter here packs its
 * primitive at this address and bumps it past what it wrote. */
#define SCRATCH (SCRATCH_PRIM_CURSOR_AS(u8))

typedef union TachometerSpriteClutAddress {
    s16 *halfwordPointer;
    SPRT *spritePointer;
} TachometerSpriteClutAddress;

typedef union TachometerColorAddress {
    u8 *components;
    s32 *packed;
} TachometerColorAddress;

void DrawWrongWayWarning(void) {
    register SPRT *packet __asm("$16");
    SPRT *next;
    s32 i;
    s32 x;
    s32 u;
    u8 *ot;
    SPRT *oldPacket;
    s32 temp;
    s32 uvOffset;
    u8 *ret;

    next = (SPRT *)SCRATCH;
    i = 0;
    u = 0x48;
    x = 0x6C;
    packet = next;

    do {
        SetSprt(next);
        SetShadeTex(next, 1);

        temp = 0x78;
        packet->y0 = temp;
        __asm__ volatile("" : :);
        uvOffset = (((i & 2) << 3) - (i & 2)) << 2;
        temp = -0x10 - uvOffset;
        uvOffset += 0x10;
        packet->u0 = temp;
        temp = 0x10;
        packet->h = temp;
        temp = 0x788C;
        oldPacket = packet;
        packet->x0 = x;
        packet->v0 = u;
        packet->w = uvOffset;
        packet->clut = temp;

        packet++;
        next++;
        u += 0x10;
        x += 0x10;
        ot = g_DrawBuffer;
        i++;
        AddPrim(ot + 0xCC, oldPacket);
    } while (i < 3);

    ret = GameQueueTileTrans(g_DrawBuffer + 0xCC, (u8 *)next, 0x64, 0x70, 0x78, 0x20, 8, 8, 8);
    SCRATCH = ret;
    SCRATCH = QueueDrawModePrim(g_DrawBuffer + 0xCC, ret, 9);
}


s32 DrawTachometer(s32 rpm, s32 flash, s32 type, s32 amt) {
    CarTachometerSpec *p = &g_CarSpec->tachometer;
    s32 cx = p->needleX;
    s32 cy = p->needleY;
    s32 b = p->angleMin;
    s32 angle = b + rpm * (p->angleMax - b) / 10000;
    s32 cos = rsin(angle);
    s32 sin = rcos(angle);
    POLY_F4 *prim = (POLY_F4 *)SCRATCH;
    s16 *vp;
    s16 *pa;
    s16 *pb;
    s32 i;
    u8 code7;

    SetPolyF4(prim);

    vp = &prim->x0;
    i = 0;
    pb = &g_TachoNeedleQuad[0][1];
    pa = pb - 1;
    for (; i < 4; i++) {
        *vp++ = cx + (sin * pa[0] - cos * pb[0]) / 4096;
        *vp++ = cy + (cos * pa[0] + sin * pb[0]) / 4096;
        pb += 2;
        pa += 2;
    }

    code7 = prim->t.code;

    if (type == 1) {
        if (amt > 96) amt = 96;
        g_TachoFaceB = -128 - amt;
        g_TachoFaceG = -128 - amt;
        g_TachoFaceR = -128 - amt;
        prim->t.r0 = (amt * 32 + p->needleColor[0] * (96 - amt)) / 96;
        prim->t.g0 = (amt * 32 + p->needleColor[1] * (96 - amt)) / 96;
        prim->t.b0 = (amt * 32 + p->needleColor[2] * (96 - amt)) / 96;
    } else if (type == 3) {
        TachometerSpriteClutAddress clutAddress;

        amt -= 32;
        if (amt < 0) amt = 0;
        g_TachoFaceB = amt + 32;
        g_TachoFaceG = amt + 32;
        g_TachoFaceR = amt + 32;
        prim->t.r0 = ((96 - amt) * 32 + p->needleColor[0] * amt) / 96;
        prim->t.g0 = ((96 - amt) * 32 + p->needleColor[1] * amt) / 96;
        prim->t.b0 = ((96 - amt) * 32 + p->needleColor[2] * amt) / 96;
        clutAddress.halfwordPointer = &((SPRT *)(g_DrawBuffer + 0x236E4))->clut;
        *clutAddress.halfwordPointer = 0x33A8;
    } else if (type == 2) {
        TachometerColorAddress packetColor;
        TachometerColorAddress needleColor;
        TachometerSpriteClutAddress clutAddress;

        clutAddress.halfwordPointer = &((SPRT *)(g_DrawBuffer + 0x236E4))->clut;
        *clutAddress.halfwordPointer = 0x33E8;
        g_TachoFaceB = 0x80;
        g_TachoFaceG = 0x80;
        g_TachoFaceR = 0x80;
        packetColor.components = &prim->t.r0;
        needleColor.components = p->needleColorAlt;
        *packetColor.packed = *needleColor.packed;
    } else {
        TachometerColorAddress packetColor;
        TachometerColorAddress needleColor;
        TachometerSpriteClutAddress clutAddress;
        s16 rv = 0x33A8;

        clutAddress.halfwordPointer = &((SPRT *)(g_DrawBuffer + 0x236E4))->clut;
        *clutAddress.halfwordPointer = rv;
        g_TachoFaceB = 0x80;
        g_TachoFaceG = 0x80;
        g_TachoFaceR = 0x80;
        packetColor.components = &prim->t.r0;
        needleColor.components = p->needleColor;
        *packetColor.packed = *needleColor.packed;
    }

    prim->t.code = code7;
    AddPrim(g_DrawBuffer + 0xCC, prim);
    prim++;
    {
        RenderBufferAddress cursor;
        cursor.polyF4 = prim;
        SCRATCH = cursor.bytes;
    }

    {
        s32 x = cx + p->gearDigitDX;
        s32 y = cy + p->gearDigitDY;
        /* Not SCRATCH: this read has to stay volatile. Spelling it as the
         * plain macro lets the cursor written above be reused instead of
         * reloaded, which changes the output. */
        u8 *q = DrawHudDigit(SCRATCH_PRIM_CURSOR_VOLATILE, x, y, g_PlayerGear, g_HudGlyphClut);
        SCRATCH = q;
        DrawSpeedDigits(cx, cy, g_PlayerSpeed * 160 / 1168);
    }

    ((SPRT *)(g_DrawBuffer + 0x236E4))->t.r0 = g_TachoFaceR;
    ((SPRT *)(g_DrawBuffer + 0x236E4))->t.g0 = g_TachoFaceG;
    ((SPRT *)(g_DrawBuffer + 0x236E4))->t.b0 = g_TachoFaceB;

    { u8 *g = g_DrawBuffer; AddPrim(g + 0xCC, g + 0x236CC); }
    { u8 *g = g_DrawBuffer; AddPrim(g + 0xCC, g + 0x236E4); }
    { u8 *g = g_DrawBuffer; AddPrim(g + 0xCC, g + 0x236D8); }

    {
        TILE *q = (TILE *)SCRATCH;
        u8 *g;
        TILE *tile;
        s32 v10;
        SetTile(q);
        tile = q;
        q->x0 = cx + p->shiftLightDX;
        v10 = cy + p->shiftLightDY;
        q->w = 0x10;
        q->h = 0x10;
        q->t.r0 = flash * 223 + 32;
        q->t.g0 = 0x20;
        q->t.b0 = 0x20;
        g = g_DrawBuffer;
        q->y0 = v10;
        q++;
        AddPrim(g + 0xCC, tile);
        {
            RenderBufferAddress cursor;
            cursor.tile = q;
            SCRATCH = cursor.bytes;
        }
    }
}

void DrawFullscreenFadeTile(s32 color, s32 tpage) {
    u8 *base = g_DrawBuffer;
    u8 *ot = base + 0xCC;
    TILE *packet;
    TILE *prim;
    s32 height;

    if (color < 0) {
        color = 0;
    } else if (color >= 0x100) {
        color = 0xFF;
    }

    packet = (TILE *)SCRATCH;
    SetTile(packet);
    SetSemiTrans(packet, 1);

    packet->w = 0x140;
    height = 0xF0;
    packet->x0 = 0;
    packet->y0 = 0;
    packet->h = height;
    packet->t.r0 = color;
    packet->t.g0 = color;
    packet->t.b0 = color;

    prim = packet;
    packet++;
    AddPrim(ot, prim);
    {
        RenderBufferAddress cursor;
        cursor.tile = packet;
        SCRATCH = QueueDrawModePrim(g_DrawBuffer + 0xCC, cursor.bytes, tpage);
    }
}

u8 *DrawHudDigit(u8 *prim, s32 x, s32 y, s32 digit, u16 clut) {
    RenderBufferAddress cursor;
    register SPRT_8 *out asm("$16");
    s32 xReg = x;
    s32 yReg = y;
    register s32 codeReg asm("$17");
    register s32 clutReg asm("$20");

    cursor.bytes = prim;
    out = cursor.sprite8;
    asm("" : "=r"(xReg), "=r"(yReg) : "0"(xReg), "1"(yReg) : "$17");
    codeReg = digit;
    clutReg = clut;
    SetSprt8(out);
    SetShadeTex(out, 1);

    codeReg <<= 3;
    out->u0 = codeReg;
    out->v0 = 0x10;

    {
        u8 *ot = g_DrawBuffer;
        register SPRT_8 *oldPrim asm("$5") = out;

        out->x0 = xReg;
        out->y0 = yReg;
        out->clut = clutReg;
        out++;
        AddPrim(ot + 0xCC, oldPrim);
    }

    {
        RenderBufferAddress cursor;
        cursor.sprite8 = out;
        return cursor.bytes;
    }
}

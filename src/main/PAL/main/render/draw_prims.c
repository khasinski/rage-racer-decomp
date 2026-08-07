#include "common.h"
#include "game/scratchpad.h"
#include "psyq/gpu.h"

void SetDrawClipRect(void *ot, s32 x, s32 y, s32 w, s32 h) {
    void *otReg;
    u8 **scratch;
    u8 *packet;
    s16 xReg;
    s16 yReg;
    s16 wReg;
    s16 hReg;
    u8 *oldPacket;
    s32 tmp;
    Rect rect;

    otReg = ot;
    xReg = x;
    yReg = y;
    wReg = w;
    scratch = &SCRATCH_PRIM_CURSOR_AS(u8);
    hReg = h;
    packet = *scratch;

    if ((s16)x + (s16)w > 0) {
        if ((s16)xReg < 0) {
            wReg = w + x;
            xReg = 0;
        }

        if ((s16)xReg < 0x140) {
            if ((s16)xReg + (s16)wReg >= 0x140) {
                tmp = 0x140;
                wReg = tmp - xReg;
            }

            if ((s16)y + (s16)h > 0) {
                if ((s16)yReg < 0) {
                    hReg = h + y;
                    yReg = 0;
                }

                if ((s16)yReg < 0x1E0) {
                    if ((s16)yReg + (s16)hReg >= 0x1E0) {
                        tmp = 0x1E0;
                        hReg = tmp - yReg;
                    }

                    rect.x = xReg;
                    rect.y = yReg;
                    rect.w = wReg;
                    rect.h = hReg;
                    SetDrawArea(packet, &rect);
                    oldPacket = packet;
                    packet += 12;
                    AddPrim(otReg, oldPacket);
                    *scratch = packet;
                }
            }
        }
    }
}

void *QueueDrawModePrim(void *ot, void *prim, s32 arg2);

void DrawSprite(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 u0, u16 v0, u8 r, u8 g, u8 b, u16 clutX, s32 shadeTex, s32 semiTrans, u32 flags) {
    SPRT *prim;
    s32 shadeReg;
    s32 semiReg;
    /* The remaining pinned local is load-bearing: removing it changes .text. */
    s32 y1Reg;
    s32 u0Reg;
    s32 v0Reg;
    s32 rReg;
    register s32 clutReg asm("$16");
    u8 *oldPrim;
    s16 x0Local;
    s16 y0Local;
    s16 x1Local;
    u8 gLocal;
    u8 bLocal;
    s32 div;
    s32 base;

    prim = SCRATCH_PRIM_CURSOR_AS(SPRT);
    shadeReg = shadeTex;
    semiReg = semiTrans;
    asm("" : : "r"(prim), "r"(shadeReg), "r"(semiReg));
    y1Reg = y1;
    u0Reg = u0;
    v0Reg = v0;
    rReg = r;
    x0Local = x0;
    y0Local = y0;
    x1Local = x1;
    bLocal = b;
    /* Match note: keep stack color load after callee-save argument setup. */
    asm("" : : "r"(flags), "r"(y1Reg), "r"(u0Reg), "r"(v0Reg), "r"(rReg));
    gLocal = g;
    SetSprt(prim);

    clutReg = clutX;
    SetShadeTex(prim, shadeReg);
    SetSemiTrans(prim, semiReg);

    prim->x0 = x0Local;
    prim->y0 = y0Local;
    prim->w = x1Local;
    prim->h = y1Reg;
    prim->u0 = u0Reg;
    prim->v0 = v0Reg;
    prim->t.r0 = rReg;
    prim->t.g0 = gLocal;
    prim->t.b0 = bLocal;

    div = (clutReg & 0xFFFF) / 20U;
    clutReg &= 0xFFFF;
    base = (div + 0x1E0) << 6;
    prim->clut = base + (clutReg - (div * 20));

    oldPrim = (u8 *)prim;
    prim++;
    AddPrim(ot, oldPrim);

    clutReg = flags;
    flags &= 0x80;
    if (flags == 0) {
        prim = QueueDrawModePrim(ot, prim, clutReg & 0xFFFF);
    }

    SCRATCH_PRIM_CURSOR_AS(SPRT) = prim;
}

void *QueueDrawModePrim(void *ot, void *prim, s32 arg2);

void DrawFlatTriangle(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 x2, u16 y2, u8 r, u8 g, u8 b, s32 semiTrans, u32 flags) {
    register s32 semiReg asm("$17");
    register u32 flagsReg asm("$16");
    s32 y1Reg;
    s32 x2Reg;
    s32 y2Reg;
    s32 rReg;
    s32 gReg;
    s32 bReg;
    POLY_F3 *prim;
    u8 *oldPrim;

    prim = SCRATCH_PRIM_CURSOR_AS(POLY_F3);
    semiReg = semiTrans;
    flagsReg = flags;
    y1Reg = y1;
    x2Reg = x2;
    y2Reg = y2;
    rReg = r;
    gReg = g;
    bReg = b;
    asm("" : : "r"(prim), "r"(semiReg), "r"(flagsReg), "r"(y1Reg), "r"(x2Reg), "r"(y2Reg), "r"(rReg), "r"(gReg), "r"(bReg) : "$4", "$5", "$6", "$7", "$31");

    SetPolyF3(prim);
    SetSemiTrans(prim, semiReg);

    prim->x0 = x0;
    prim->y0 = y0;
    prim->x1 = x1;
    prim->y1 = y1Reg;
    prim->x2 = x2Reg;
    prim->y2 = y2Reg;
    prim->t.r0 = rReg;
    prim->t.g0 = gReg;
    prim->t.b0 = bReg;

    oldPrim = (u8 *)prim;
    prim++;
    AddPrim((u32 *)ot, (u32 *)oldPrim);

    semiReg = flagsReg;
    flagsReg &= 0x80;
    if (flagsReg == 0) {
        prim = QueueDrawModePrim(ot, prim, semiReg & 0xFFFF);
    }

    SCRATCH_PRIM_CURSOR_AS(POLY_F3) = prim;
}

void *QueueDrawModePrim(void *ot, void *prim, s32 arg2);

void DrawFlatQuad(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u8 r, u8 g, u8 b, s32 semiTrans, u32 flags) {
    POLY_F4 *prim;
    register s32 semiReg asm("$17");
    register u32 flagsReg asm("$16");
    s32 y1Reg;
    s32 x2Reg;
    s32 y2Reg;
    s32 x3Reg;
    s32 y3Reg;
    s32 rReg;
    u8 *oldPrim;
    s16 x0Local;
    s16 y0Local;
    s16 x1Local;
    u8 gLocal;
    u8 bLocal;

    prim = SCRATCH_PRIM_CURSOR_AS(POLY_F4);
    semiReg = semiTrans;
    flagsReg = flags;
    y1Reg = y1;
    x2Reg = x2;
    y2Reg = y2;
    x3Reg = x3;
    y3Reg = y3;
    rReg = r;
    x0Local = x0;
    y0Local = y0;
    x1Local = x1;
    bLocal = b;
    /* Match note: keep stack color load after callee-save argument setup. */
    asm("" : : "r"(prim), "r"(semiReg), "r"(flagsReg), "r"(y1Reg), "r"(x2Reg), "r"(y2Reg), "r"(x3Reg), "r"(y3Reg), "r"(rReg) : "$4", "$5", "$6", "$7", "$31");
    gLocal = g;

    SetPolyF4(prim);
    SetSemiTrans(prim, semiReg);

    prim->x0 = x0Local;
    prim->y0 = y0Local;
    prim->x1 = x1Local;
    prim->y1 = y1Reg;
    prim->x2 = x2Reg;
    prim->y2 = y2Reg;
    prim->x3 = x3Reg;
    prim->y3 = y3Reg;
    prim->t.r0 = rReg;
    prim->t.g0 = gLocal;
    prim->t.b0 = bLocal;

    oldPrim = (u8 *)prim;
    prim++;
    AddPrim(ot, oldPrim);

    semiReg = flagsReg;
    flagsReg &= 0x80;
    if (flagsReg == 0) {
        prim = QueueDrawModePrim(ot, prim, semiReg & 0xFFFF);
    }

    SCRATCH_PRIM_CURSOR_AS(POLY_F4) = prim;
}

/*
 * Packs a POLY_FT4 (textured quad) at the scratchpad cursor and links it into
 * the ordering table. clutIndex is a linear palette slot turned into VRAM clut
 * coordinates: 20 cluts per row starting at y = 0x1E0.
 */
void GameDrawTexturedQuad(s32 ot, s16 x0, s16 y0, s16 x1,
                   u16 y1, u16 x2, u16 y2, u16 x3, u16 y3,
                   u8 u0, u8 v0, u8 u1, u8 v1, u8 u2, u8 v2,
                   u8 u3, u8 v3, u8 r, u8 g, u8 b,
                   u16 clutIndex, s32 shadeTex, s32 semiTrans, u16 tpage);
void GameDrawTexturedQuad(s32 ot, s16 x0, s16 y0, s16 x1,
                   u16 y1, u16 x2, u16 y2, u16 x3, u16 y3,
                   u8 u0, u8 v0, u8 u1, u8 v1, u8 u2, u8 v2,
                   u8 u3, u8 v3, u8 r, u8 g, u8 b,
                   u16 clutIndex, s32 shadeTex, s32 semiTrans, u16 tpage) {
    POLY_FT4 *prim = SCRATCH_PRIM_CURSOR_AS(POLY_FT4);
    u32 d;
    u32 clutRow;
    u32 rem;
    s32 clut;
    u8 *oldPrim;

    SetPolyFT4(prim);
    SetShadeTex(prim, shadeTex);
    SetSemiTrans(prim, semiTrans);
    prim->x0 = x0;
    prim->y0 = y0;
    prim->x1 = x1;
    prim->y1 = y1;
    prim->x2 = x2;
    prim->y2 = y2;
    prim->x3 = x3;
    prim->y3 = y3;
    prim->u0 = u0;
    prim->v0 = v0;
    prim->u1 = u1;
    prim->v1 = v1;
    prim->u2 = u2;
    prim->v2 = v2;
    prim->u3 = u3;
    prim->v3 = v3;
    prim->t.r0 = r;
    prim->t.g0 = g;
    prim->t.b0 = b;
    prim->tpage = tpage;
    d = clutIndex;
    clutRow = d / 20;
    clut = (clutRow + 0x1E0) << 6;
    rem = d - clutRow * 20;
    clut = clut + rem;
    prim->clut = clut;
    oldPrim = (u8 *)prim;
    prim++;
    AddPrim((u32 *)ot, (u32 *)oldPrim);
    SCRATCH_PRIM_CURSOR_AS(POLY_FT4) = prim;
}

void *QueueDrawModePrim(void *ot, void *prim, s32 arg2);

void DrawSolidRect(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha) {
    s32 x0Reg;
    s32 y0Reg;
    s32 x1Reg;
    s32 y1Reg;
    s32 rReg;
    s32 gReg;
    s32 bReg;
    register u8 alphaReg asm("$16");
    u8 *a0Reg;
    TILE *prim;
    u8 *oldPrim;

    prim = SCRATCH_PRIM_CURSOR_AS(TILE);
    y1Reg = y1;
    rReg = r;
    gReg = g;
    bReg = b;
    alphaReg = alpha;
    asm("");
    x0Reg = x0;
    y0Reg = y0;
    x1Reg = x1;
    asm("" : : "r"(x0Reg), "r"(y0Reg), "r"(x1Reg));

    SetTile(prim);
    a0Reg = (u8 *)prim;
    SetSemiTrans(a0Reg, alphaReg != 0xFF);

    prim->x0 = x0Reg;
    prim->y0 = y0Reg;
    prim->w = x1Reg;
    prim->h = y1Reg;
    prim->t.r0 = rReg;
    prim->t.g0 = gReg;
    prim->t.b0 = bReg;

    oldPrim = (u8 *)prim;
    prim++;
    AddPrim((u32 *)ot, (u32 *)oldPrim);

    if (alphaReg != 0xFF) {
        prim = QueueDrawModePrim(ot, prim, alphaReg);
    }

    SCRATCH_PRIM_CURSOR_AS(TILE) = prim;
}

void *QueueDrawModePrim(void *ot, void *prim, s32 arg2);

void DrawLine(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha) {
    s32 x0Reg;
    s32 y0Reg;
    s32 x1Reg;
    s32 y1Reg;
    s32 rReg;
    s32 gReg;
    s32 bReg;
    register u8 alphaReg asm("$16");
    u8 *a0Reg;
    LINE_F2 *prim;
    u8 *oldPrim;

    prim = SCRATCH_PRIM_CURSOR_AS(LINE_F2);
    y1Reg = y1;
    rReg = r;
    gReg = g;
    bReg = b;
    alphaReg = alpha;
    asm("");
    x0Reg = x0;
    y0Reg = y0;
    x1Reg = x1;
    asm("" : : "r"(x0Reg), "r"(y0Reg), "r"(x1Reg));

    SetLineF2(prim);
    a0Reg = (u8 *)prim;
    SetSemiTrans(a0Reg, alphaReg != 0xFF);

    prim->x0 = x0Reg;
    prim->y0 = y0Reg;
    prim->x1 = x1Reg;
    prim->y1 = y1Reg;
    prim->t.r0 = rReg;
    prim->t.g0 = gReg;
    prim->t.b0 = bReg;

    oldPrim = (u8 *)prim;
    prim++;
    AddPrim((u32 *)ot, (u32 *)oldPrim);

    if (alphaReg != 0xFF) {
        prim = QueueDrawModePrim(ot, prim, alphaReg);
    }

    SCRATCH_PRIM_CURSOR_AS(LINE_F2) = prim;
}

void *QueueDrawModePrim(void *ot, void *prim, s32 arg2);

void DrawPolyLine3(void *ot, s16 x0, s16 y0, s16 x1, s16 y1, s16 x2, s16 y2, u8 r, u8 g, u8 b, u8 arg10) {
    LINE_F3 *prim;
    u8 *oldPrim;

    prim = SCRATCH_PRIM_CURSOR_AS(LINE_F3);
    SetLineF3(prim);
    SetSemiTrans(prim, arg10 != 0xFF);

    prim->x0 = x0;
    prim->y0 = y0;
    prim->x1 = x1;
    prim->y1 = y1;
    prim->x2 = x2;
    prim->y2 = y2;
    prim->t.r0 = r;
    prim->t.g0 = g;
    prim->t.b0 = b;

    oldPrim = (u8 *)prim;
    prim++;
    AddPrim(ot, oldPrim);

    if (arg10 != 0xFF) {
        prim = QueueDrawModePrim(ot, prim, arg10);
    }

    SCRATCH_PRIM_CURSOR_AS(LINE_F3) = prim;
}

void *QueueDrawModePrim(void *ot, void *prim, s32 arg2);

void DrawGradientLine(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r0, u8 g0, u8 b0, u8 r1, u8 g1, u8 b1, u8 alpha) {
    s16 x0Reg;
    s16 y0Reg;
    s16 x1Reg;
    s32 y1Reg;
    s32 r0Reg;
    s32 g0Reg;
    s32 b0Reg;
    u8 alphaReg;
    u8 *a0Reg;
    LINE_G2 *prim;
    u8 *oldPrim;
    u8 r1Local;
    u8 g1Local;
    u8 b1Local;

    prim = SCRATCH_PRIM_CURSOR_AS(LINE_G2);
    y1Reg = y1;
    r0Reg = r0;
    g0Reg = g0;
    b0Reg = b0;
    asm("" : : "r"(y1Reg), "r"(r0Reg), "r"(g0Reg), "r"(b0Reg));
    r1Local = r1;
    alphaReg = alpha;
    x0Reg = x0;
    y0Reg = y0;
    x1Reg = x1;
    g1Local = g1;
    b1Local = b1;

    SetLineG2(prim);
    a0Reg = (u8 *)prim;
    SetSemiTrans(a0Reg, alphaReg != 0xFF);

    prim->x0 = x0Reg;
    prim->y0 = y0Reg;
    prim->x1 = x1Reg;
    prim->y1 = y1Reg;
    prim->t.r0 = r0Reg;
    prim->t.g0 = g0Reg;
    prim->t.b0 = b0Reg;
    prim->r1 = r1Local;
    prim->g1 = g1Local;
    prim->b1 = b1Local;

    oldPrim = (u8 *)prim;
    prim++;
    AddPrim((u32 *)ot, (u32 *)oldPrim);

    if (alphaReg != 0xFF) {
        prim = QueueDrawModePrim(ot, prim, alphaReg);
    }

    SCRATCH_PRIM_CURSOR_AS(LINE_G2) = prim;
}

void GameDrawLineWide(void *buf, s16 x1, s16 y1, s16 x2, s16 y2, s32 r, s32 g, s32 b, s32 code) asm("DrawLine");
void DrawRectOutline(void *buf, s32 xa, s32 ya, s32 w, s32 h, u8 r, u8 g, u8 b, u8 code);
void DrawRectOutline(void *buf, s32 xa, s32 ya, s32 w, s32 h, u8 r, u8 g, u8 b, u8 code)
{
  s32 x_R19 = xa;
  s32 y_R18 = ya;
  s16 new_var2;
  s32 x0;
  int new_var4;
  s32 x1;
  s32 ytop2;
  s32 new_var3;
  s32 ybot;
  s16 new_var;
  s32 ybi;
  new_var = (s16) x_R19;
  x0 = new_var;
  new_var3 = x_R19;
  new_var4 = w - 1;
  x1 = (s16) (new_var3 + new_var4);
  new_var2 = (s16) y_R18;
  h = (((((((((h - 1) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu;
  GameDrawLineWide(buf, x0, new_var2, x1, new_var2, r, g, b, code);
  GameDrawLineWide(buf, x0, (s16) (y_R18 + 1), x1, (s16) (y_R18 + 1), r, g, b, code);
  ytop2 = (s16) (y_R18 + 2);
  ybot = y_R18 + h;
  ybi = (s16) (ybot - 2);
  GameDrawLineWide(buf, x0, ytop2, x0, ybi, r, g, b, code);
  GameDrawLineWide(buf, x1, ytop2, x1, ybi, r, g, b, code);
  GameDrawLineWide(buf, x0, (s16) ybot, x1, (s16) ybot, r, g, b, code);
  GameDrawLineWide(buf, x0, (s16) (ybot - 1), x1, (s16) (ybot - 1), r, g, b, code);
}

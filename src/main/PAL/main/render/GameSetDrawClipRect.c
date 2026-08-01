#include "common.h"
#include "psyq/gpu.h"

void func_80066604(void *packet, void *rect);
void AddPrim(void *ot, void *prim) asm("func_80064DDC");

void GameSetDrawClipRect(void *ot, s32 x, s32 y, s32 w, s32 h) asm("func_800468FC");
void GameSetDrawClipRect(void *ot, s32 x, s32 y, s32 w, s32 h) {
    void *otReg;
    u8 **scratch;
    u8 *packet;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 xReg asm("$9");
    register s32 yReg asm("$10");
    register s32 wReg asm("$11");
    register s32 hReg asm("$8");
    u8 *oldPacket;
    s32 tmp;
    Rect rect;

    otReg = ot;
    xReg = x;
    yReg = y;
    wReg = w;
    scratch = (u8 **)0x1F800000;
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
                    func_80066604(packet, &rect);
                    oldPacket = packet;
                    packet += 12;
                    AddPrim(otReg, oldPacket);
                    *scratch = packet;
                }
            }
        }
    }
}

void func_80064FA8(void *arg0);
void func_80064EB8(void *arg0, s32 enabled);
void func_80064E90(void *arg0, s32 enabled);
void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawSprite(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 u0, u16 v0, u8 r, u8 g, u8 b, u16 clutX, s32 shadeTex, s32 semiTrans, u32 flags) asm("func_80046A2C");
void GameDrawSprite(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 u0, u16 v0, u8 r, u8 g, u8 b, u16 clutX, s32 shadeTex, s32 semiTrans, u32 flags) {
    SPRT *prim;
    s32 shadeReg;
    s32 semiReg;
    /* These pins are load-bearing: removing any one changes .text. */
    register u32 flagsReg asm("$17");
    register s32 y1Reg asm("$20");
    register s32 u0Reg asm("$21");
    register s32 v0Reg asm("$22");
    register s32 rReg asm("$23");
    register s32 clutReg asm("$16");
    u8 *oldPrim;
    s16 x0Local;
    s16 y0Local;
    s16 x1Local;
    u8 gLocal;
    u8 bLocal;
    s32 div;
    s32 base;
    u32 magic;

    prim = *(SPRT **)0x1F800000;
    shadeReg = shadeTex;
    semiReg = semiTrans;
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : : "r"(prim), "r"(shadeReg), "r"(semiReg));
    flagsReg = flags;
    y1Reg = y1;
    u0Reg = u0;
    v0Reg = v0;
    rReg = r;
    x0Local = x0;
    y0Local = y0;
    x1Local = x1;
    bLocal = b;
    /* Match note: keep stack color load after callee-save argument setup. */
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : : "r"(flagsReg), "r"(y1Reg), "r"(u0Reg), "r"(v0Reg), "r"(rReg));
    gLocal = g;
    func_80064FA8(prim);

    clutReg = clutX;
    func_80064EB8(prim, shadeReg);
    func_80064E90(prim, semiReg);

    prim->x0 = x0Local;
    prim->y0 = y0Local;
    prim->w = x1Local;
    prim->h = y1Reg;
    prim->u0 = u0Reg;
    prim->v0 = v0Reg;
    prim->t.r0 = rReg;
    prim->t.g0 = gLocal;
    prim->t.b0 = bLocal;

    /* These barriers are load-bearing: removing any one changes .text. */
    asm("" : "=r"(clutReg) : "0"(clutReg));
    magic = 0xCCCCCCCD;
    asm("" : : "r"(magic));
    clutReg &= 0xFFFF;
    div = clutReg / 20U;
    base = (div + 0x1E0) << 6;
    prim->clut = base + (clutReg - (div * 20));

    oldPrim = (u8 *)prim;
    prim++;
    AddPrim(ot, oldPrim);

    clutReg = flagsReg;
    flagsReg &= 0x80;
    if (flagsReg == 0) {
        prim = func_80017390(ot, prim, clutReg & 0xFFFF);
    }

    *(SPRT **)0x1F800000 = prim;
}

void func_80064EE0(void *arg0);
void SetSemiTrans(void *arg0, long enabled) asm("func_80064E90");
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawFlatTriangle(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 x2, u16 y2, u8 r, u8 g, u8 b, s32 semiTrans, u32 arg11) asm("func_80046BA0");
void GameDrawFlatTriangle(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 x2, u16 y2, u8 r, u8 g, u8 b, s32 semiTrans, u32 arg11) {
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 semiReg asm("$17");
    register u32 flagsReg asm("$16");
    register s32 y1Reg asm("$18");
    register s32 x2Reg asm("$19");
    register s32 y2Reg asm("$20");
    register s32 rReg asm("$21");
    register s32 gReg asm("$22");
    register s32 bReg asm("$23");
    POLY_F3 *prim;
    u8 *oldPrim;

    prim = *(POLY_F3 **)0x1F800000;
    semiReg = semiTrans;
    flagsReg = arg11;
    y1Reg = y1;
    x2Reg = x2;
    y2Reg = y2;
    rReg = r;
    gReg = g;
    bReg = b;

    func_80064EE0(prim);
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
        prim = func_80017390(ot, prim, semiReg & 0xFFFF);
    }

    *(POLY_F3 **)0x1F800000 = prim;
}

void func_80064F30(void *arg0);
void func_80064E90(void *arg0, s32 enabled);
void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawFlatQuad(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u8 r, u8 g, u8 b, s32 semiTrans, u32 flags) asm("func_80046CBC");
void GameDrawFlatQuad(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u8 r, u8 g, u8 b, s32 semiTrans, u32 flags) {
    POLY_F4 *prim;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 semiReg asm("$17");
    register u32 flagsReg asm("$16");
    register s32 y1Reg asm("$18");
    register s32 x2Reg asm("$19");
    register s32 y2Reg asm("$20");
    register s32 x3Reg asm("$21");
    register s32 y3Reg asm("$22");
    register s32 rReg asm("$23");
    u8 *oldPrim;
    s16 x0Local;
    s16 y0Local;
    s16 x1Local;
    u8 gLocal;
    u8 bLocal;

    prim = *(POLY_F4 **)0x1F800000;
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
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : : "r"(prim), "r"(semiReg), "r"(flagsReg), "r"(y1Reg), "r"(x2Reg), "r"(y2Reg), "r"(x3Reg), "r"(y3Reg), "r"(rReg));
    gLocal = g;

    func_80064F30(prim);
    func_80064E90(prim, semiReg);

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
        prim = func_80017390(ot, prim, semiReg & 0xFFFF);
    }

    *(POLY_F4 **)0x1F800000 = prim;
}

void SetPolyFT4(void *prim) asm("func_80064F44");
void SetShadeTex(void *prim, long enabled) asm("func_80064EB8");
void SetSemiTrans(void *prim, long enabled) asm("func_80064E90");

/*
 * Packs a POLY_FT4 (textured quad) at the scratchpad cursor and links it into
 * the ordering table. clutIndex is a linear palette slot turned into VRAM clut
 * coordinates: 20 cluts per row starting at y = 0x1E0.
 */
void GameDrawTexturedQuad(s32 ot, s16 x0, s16 y0, s16 x1,
                   u16 y1, u16 x2, u16 y2, u16 x3, u16 y3,
                   u8 u0, u8 v0, u8 u1, u8 v1, u8 u2, u8 v2,
                   u8 u3, u8 v3, u8 r, u8 g, u8 b,
                   u16 clutIndex, s32 shadeTex, s32 semiTrans, u16 tpage) asm("func_80046E00");
void GameDrawTexturedQuad(s32 ot, s16 x0, s16 y0, s16 x1,
                   u16 y1, u16 x2, u16 y2, u16 x3, u16 y3,
                   u8 u0, u8 v0, u8 u1, u8 v1, u8 u2, u8 v2,
                   u8 u3, u8 v3, u8 r, u8 g, u8 b,
                   u16 clutIndex, s32 shadeTex, s32 semiTrans, u16 tpage) {
    POLY_FT4 *prim = *(POLY_FT4 **)0x1F800000;
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
    *(POLY_FT4 **)0x1F800000 = prim;
}

void func_80064FF8(void *arg0);
void SetSemiTrans(void *arg0, long enabled) asm("func_80064E90");
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawSolidRect(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha) asm("func_80047024");
void GameDrawSolidRect(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha) {
    s32 x0Reg;
    s32 y0Reg;
    s32 x1Reg;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 y1Reg asm("$20");
    register s32 rReg asm("$21");
    register s32 gReg asm("$22");
    register s32 bReg asm("$23");
    register u8 alphaReg asm("$16");
    u8 *a0Reg;
    TILE *prim;
    u8 *oldPrim;

    prim = *(TILE **)0x1F800000;
    y1Reg = y1;
    rReg = r;
    gReg = g;
    bReg = b;
    alphaReg = alpha;
    x0Reg = x0;
    y0Reg = y0;
    x1Reg = x1;

    func_80064FF8(prim);
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
        prim = func_80017390(ot, prim, alphaReg);
    }

    *(TILE **)0x1F800000 = prim;
}

void func_8006500C(void *arg0);
void SetSemiTrans(void *arg0, long enabled) asm("func_80064E90");
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawLine(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha) asm("func_8004711C");
void GameDrawLine(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha) {
    s32 x0Reg;
    s32 y0Reg;
    s32 x1Reg;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 y1Reg asm("$20");
    register s32 rReg asm("$21");
    register s32 gReg asm("$22");
    register s32 bReg asm("$23");
    register u8 alphaReg asm("$16");
    u8 *a0Reg;
    LINE_F2 *prim;
    u8 *oldPrim;

    prim = *(LINE_F2 **)0x1F800000;
    y1Reg = y1;
    rReg = r;
    gReg = g;
    bReg = b;
    alphaReg = alpha;
    x0Reg = x0;
    y0Reg = y0;
    x1Reg = x1;

    func_8006500C(prim);
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
        prim = func_80017390(ot, prim, alphaReg);
    }

    *(LINE_F2 **)0x1F800000 = prim;
}

void func_80065034(void *arg0);
void func_80064E90(void *arg0, s32 enabled);
void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawPolyLine3(void *ot, s16 x0, s16 y0, s16 x1, s16 y1, s16 x2, s16 y2, u8 r, u8 g, u8 b, u8 arg10) asm("func_80047214");
void GameDrawPolyLine3(void *ot, s16 x0, s16 y0, s16 x1, s16 y1, s16 x2, s16 y2, u8 r, u8 g, u8 b, u8 arg10) {
    LINE_F3 *prim;
    u8 *oldPrim;

    prim = *(LINE_F3 **)0x1F800000;
    func_80065034(prim);
    func_80064E90(prim, arg10 != 0xFF);

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
        prim = func_80017390(ot, prim, arg10);
    }

    *(LINE_F3 **)0x1F800000 = prim;
}

void func_80065020(void *arg0);
void SetSemiTrans(void *arg0, long enabled) asm("func_80064E90");
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawGradientLine(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r0, u8 g0, u8 b0, u8 r1, u8 g1, u8 b1, u8 alpha) asm("func_80047330");
void GameDrawGradientLine(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r0, u8 g0, u8 b0, u8 r1, u8 g1, u8 b1, u8 alpha) {
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 x0Reg asm("$17");
    register s32 y0Reg asm("$18");
    register s32 x1Reg asm("$19");
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

    prim = *(LINE_G2 **)0x1F800000;
    y1Reg = y1;
    r0Reg = r0;
    g0Reg = g0;
    b0Reg = b0;
    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : : "r"(y1Reg), "r"(r0Reg), "r"(g0Reg), "r"(b0Reg));
    r1Local = r1;
    alphaReg = alpha;
    x0Reg = x0;
    y0Reg = y0;
    x1Reg = x1;
    g1Local = g1;
    b1Local = b1;

    func_80065020(prim);
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
        prim = func_80017390(ot, prim, alphaReg);
    }

    *(LINE_G2 **)0x1F800000 = prim;
}

void func_8004711C(void *buf, s16 x1, s16 y1, s16 x2, s16 y2, s32 r, s32 g, s32 b, s32 code);
void GameDrawRectOutline(void *buf, s32 xa, s32 ya, s32 w, s32 h, u8 r, u8 g, u8 b, u8 code) asm("func_80047460");
void GameDrawRectOutline(void *buf, s32 xa, s32 ya, s32 w, s32 h, u8 r, u8 g, u8 b, u8 code)
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
  func_8004711C(buf, x0, new_var2, x1, new_var2, r, g, b, code);
  func_8004711C(buf, x0, (s16) (y_R18 + 1), x1, (s16) (y_R18 + 1), r, g, b, code);
  ytop2 = (s16) (y_R18 + 2);
  ybot = y_R18 + h;
  ybi = (s16) (ybot - 2);
  func_8004711C(buf, x0, ytop2, x0, ybi, r, g, b, code);
  func_8004711C(buf, x1, ytop2, x1, ybi, r, g, b, code);
  func_8004711C(buf, x0, (s16) ybot, x1, (s16) ybot, r, g, b, code);
  func_8004711C(buf, x0, (s16) (ybot - 1), x1, (s16) (ybot - 1), r, g, b, code);
}

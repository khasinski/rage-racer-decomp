#include "common.h"
#include "psyq/gpu.h"

void func_80066604(void *packet, void *rect);
void func_80064DDC(void *ot, void *packet);

void GameSetDrawClipRect(void *ot, s32 x, s32 y, s32 w, s32 h) asm("func_800468FC");
void GameSetDrawClipRect(void *ot, s32 x, s32 y, s32 w, s32 h) {
    register void *otReg asm("$17");
    register u8 **scratch asm("$18");
    register u8 *packet asm("$16");
    register s32 xReg asm("$9");
    register s32 yReg asm("$10");
    register s32 wReg asm("$11");
    register s32 hReg asm("$8");
    register u8 *oldPacket asm("$5");
    register s32 tmp asm("$2");
    Rect rect;

    otReg = ot;
    asm("" : "=r"(otReg) : "0"(otReg));
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
                    func_80064DDC(otReg, oldPacket);
                    *scratch = packet;
                }
            }
        }
    }
}

void func_80064FA8(u8 *arg0);
void func_80064EB8(u8 *arg0, s32 enabled);
void func_80064E90(u8 *arg0, s32 enabled);
void func_80064DDC(void *ot, void *prim);
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawSprite(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 u0, u16 v0, u8 r, u8 g, u8 b, u16 clutX, s32 shadeTex, s32 semiTrans, u32 flags) asm("func_80046A2C");
void GameDrawSprite(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 u0, u16 v0, u8 r, u8 g, u8 b, u16 clutX, s32 shadeTex, s32 semiTrans, u32 flags) {
    u8 *prim;
    register s32 shadeReg asm("$18");
    register s32 semiReg asm("$19");
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

    prim = *(u8 **)0x1F800000;
    shadeReg = shadeTex;
    semiReg = semiTrans;
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
    asm("" : : "r"(flagsReg), "r"(y1Reg), "r"(u0Reg), "r"(v0Reg), "r"(rReg));
    gLocal = g;
    asm("" : : "r"(gLocal));
    clutReg = clutX;

    func_80064FA8(prim);
    func_80064EB8(prim, shadeReg);
    func_80064E90(prim, semiReg);

    *(s16 *)(prim + 0x8) = x0Local;
    *(s16 *)(prim + 0xA) = y0Local;
    *(s16 *)(prim + 0x10) = x1Local;
    *(s16 *)(prim + 0x12) = y1Reg;
    prim[0xC] = u0Reg;
    prim[0xD] = v0Reg;
    prim[0x4] = rReg;
    prim[0x5] = gLocal;
    prim[0x6] = bLocal;

    asm("" : "=r"(clutReg) : "0"(clutReg));
    magic = 0xCCCCCCCD;
    asm("" : : "r"(magic));
    clutReg &= 0xFFFF;
    div = clutReg / 20U;
    base = (div + 0x1E0) << 6;
    *(s16 *)(prim + 0xE) = base + (clutReg - (div * 20));

    oldPrim = prim;
    prim += 0x14;
    func_80064DDC(ot, oldPrim);

    clutReg = flagsReg;
    flagsReg &= 0x80;
    if (flagsReg == 0) {
        prim = func_80017390(ot, prim, clutReg & 0xFFFF);
    }

    *(u8 **)0x1F800000 = prim;
}

void func_80064EE0(u8 *arg0);
void SetSemiTrans(u8 *arg0, s32 enabled) asm("func_80064E90");
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawFlatTriangle(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 x2, u16 y2, u8 r, u8 g, u8 b, s32 semiTrans, u32 arg11) asm("func_80046BA0");
void GameDrawFlatTriangle(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 x2, u16 y2, u8 r, u8 g, u8 b, s32 semiTrans, u32 arg11) {
    register s32 semiReg asm("$17");
    register u32 flagsReg asm("$16");
    register s32 y1Reg asm("$18");
    register s32 x2Reg asm("$19");
    register s32 y2Reg asm("$20");
    register s32 rReg asm("$21");
    register s32 gReg asm("$22");
    register s32 bReg asm("$23");
    u8 *prim;
    u8 *oldPrim;

    prim = *(u8 **)0x1F800000;
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

    *(s16 *)(prim + 0x8) = x0;
    *(s16 *)(prim + 0xA) = y0;
    *(s16 *)(prim + 0xC) = x1;
    *(s16 *)(prim + 0xE) = y1Reg;
    *(s16 *)(prim + 0x10) = x2Reg;
    *(s16 *)(prim + 0x12) = y2Reg;
    prim[0x4] = rReg;
    prim[0x5] = gReg;
    prim[0x6] = bReg;

    oldPrim = prim;
    prim += 0x14;
    AddPrim((u32 *)ot, (u32 *)oldPrim);

    semiReg = flagsReg;
    flagsReg &= 0x80;
    if (flagsReg == 0) {
        prim = func_80017390(ot, prim, semiReg & 0xFFFF);
    }

    *(u8 **)0x1F800000 = prim;
}

void func_80064F30(u8 *arg0);
void func_80064E90(u8 *arg0, s32 enabled);
void func_80064DDC(void *ot, void *prim);
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawFlatQuad(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u8 r, u8 g, u8 b, s32 semiTrans, u32 flags) asm("func_80046CBC");
void GameDrawFlatQuad(void *ot, s16 x0, s16 y0, s16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u8 r, u8 g, u8 b, s32 semiTrans, u32 flags) {
    u8 *prim;
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

    prim = *(u8 **)0x1F800000;
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
    asm("" : : "r"(prim), "r"(semiReg), "r"(flagsReg), "r"(y1Reg), "r"(x2Reg), "r"(y2Reg), "r"(x3Reg), "r"(y3Reg), "r"(rReg));
    gLocal = g;

    func_80064F30(prim);
    func_80064E90(prim, semiReg);

    *(s16 *)(prim + 0x8) = x0Local;
    *(s16 *)(prim + 0xA) = y0Local;
    *(s16 *)(prim + 0xC) = x1Local;
    *(s16 *)(prim + 0xE) = y1Reg;
    *(s16 *)(prim + 0x10) = x2Reg;
    *(s16 *)(prim + 0x12) = y2Reg;
    *(s16 *)(prim + 0x14) = x3Reg;
    *(s16 *)(prim + 0x16) = y3Reg;
    prim[0x4] = rReg;
    prim[0x5] = gLocal;
    prim[0x6] = bLocal;

    oldPrim = prim;
    prim += 0x18;
    func_80064DDC(ot, oldPrim);

    semiReg = flagsReg;
    flagsReg &= 0x80;
    if (flagsReg == 0) {
        prim = func_80017390(ot, prim, semiReg & 0xFFFF);
    }

    *(u8 **)0x1F800000 = prim;
}

void SetPolyFT4(u8 *prim) asm("func_80064F44");
void SetShadeTex(u8 *prim, s32 enabled) asm("func_80064EB8");
void SetSemiTrans(u8 *prim, s32 enabled) asm("func_80064E90");

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
    u8 *prim = *(u8 **)0x1F800000;
    u32 d;
    u32 clutRow;
    u32 rem;
    s32 clut;
    u8 *oldPrim;

    SetPolyFT4(prim);
    SetShadeTex(prim, shadeTex);
    SetSemiTrans(prim, semiTrans);
    *(u16 *)(prim + 0x8) = x0;
    *(u16 *)(prim + 0xA) = y0;
    *(u16 *)(prim + 0x10) = x1;
    *(u16 *)(prim + 0x12) = y1;
    *(u16 *)(prim + 0x18) = x2;
    *(u16 *)(prim + 0x1A) = y2;
    *(u16 *)(prim + 0x20) = x3;
    *(u16 *)(prim + 0x22) = y3;
    prim[0xC] = u0;
    prim[0xD] = v0;
    prim[0x14] = u1;
    prim[0x15] = v1;
    prim[0x1C] = u2;
    prim[0x1D] = v2;
    prim[0x24] = u3;
    prim[0x25] = v3;
    prim[0x4] = r;
    prim[0x5] = g;
    prim[0x6] = b;
    *(u16 *)(prim + 0x16) = tpage;
    d = clutIndex;
    clutRow = d / 20;
    clut = (clutRow + 0x1E0) << 6;
    rem = d - clutRow * 20;
    clut = clut + rem;
    *(u16 *)(prim + 0xE) = clut;
    oldPrim = prim;
    prim += 0x28;
    AddPrim((u32 *)ot, (u32 *)oldPrim);
    *(u8 **)0x1F800000 = prim;
}

void func_80064FF8(u8 *arg0);
void SetSemiTrans(u8 *arg0, s32 enabled) asm("func_80064E90");
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawSolidRect(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha) asm("func_80047024");
void GameDrawSolidRect(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha) {
    register s32 x0Reg asm("$17");
    register s32 y0Reg asm("$18");
    register s32 x1Reg asm("$19");
    register s32 y1Reg asm("$20");
    register s32 rReg asm("$21");
    register s32 gReg asm("$22");
    register s32 bReg asm("$23");
    register u8 alphaReg asm("$16");
    register u8 *a0Reg asm("$4");
    u8 *prim;
    u8 *oldPrim;

    prim = *(u8 **)0x1F800000;
    y1Reg = y1;
    rReg = r;
    gReg = g;
    bReg = b;
    alphaReg = alpha;
    x0Reg = x0;
    y0Reg = y0;
    x1Reg = x1;

    func_80064FF8(prim);
    a0Reg = prim;
    SetSemiTrans(a0Reg, alphaReg != 0xFF);

    *(s16 *)(prim + 0x8) = x0Reg;
    *(s16 *)(prim + 0xA) = y0Reg;
    *(s16 *)(prim + 0xC) = x1Reg;
    *(s16 *)(prim + 0xE) = y1Reg;
    prim[0x4] = rReg;
    prim[0x5] = gReg;
    prim[0x6] = bReg;

    oldPrim = prim;
    prim += 0x10;
    AddPrim((u32 *)ot, (u32 *)oldPrim);

    if (alphaReg != 0xFF) {
        prim = func_80017390(ot, prim, alphaReg);
    }

    *(u8 **)0x1F800000 = prim;
}

void func_8006500C(u8 *arg0);
void SetSemiTrans(u8 *arg0, s32 enabled) asm("func_80064E90");
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawLine(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha) asm("func_8004711C");
void GameDrawLine(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r, u8 g, u8 b, u8 alpha) {
    register s32 x0Reg asm("$17");
    register s32 y0Reg asm("$18");
    register s32 x1Reg asm("$19");
    register s32 y1Reg asm("$20");
    register s32 rReg asm("$21");
    register s32 gReg asm("$22");
    register s32 bReg asm("$23");
    register u8 alphaReg asm("$16");
    register u8 *a0Reg asm("$4");
    u8 *prim;
    u8 *oldPrim;

    prim = *(u8 **)0x1F800000;
    y1Reg = y1;
    rReg = r;
    gReg = g;
    bReg = b;
    alphaReg = alpha;
    x0Reg = x0;
    y0Reg = y0;
    x1Reg = x1;

    func_8006500C(prim);
    a0Reg = prim;
    SetSemiTrans(a0Reg, alphaReg != 0xFF);

    *(s16 *)(prim + 0x8) = x0Reg;
    *(s16 *)(prim + 0xA) = y0Reg;
    *(s16 *)(prim + 0xC) = x1Reg;
    *(s16 *)(prim + 0xE) = y1Reg;
    prim[0x4] = rReg;
    prim[0x5] = gReg;
    prim[0x6] = bReg;

    oldPrim = prim;
    prim += 0x10;
    AddPrim((u32 *)ot, (u32 *)oldPrim);

    if (alphaReg != 0xFF) {
        prim = func_80017390(ot, prim, alphaReg);
    }

    *(u8 **)0x1F800000 = prim;
}

void func_80065034(u8 *arg0);
void func_80064E90(u8 *arg0, s32 enabled);
void func_80064DDC(void *ot, void *prim);
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawPolyLine3(void *ot, s16 x0, s16 y0, s16 x1, s16 y1, s16 x2, s16 y2, u8 r, u8 g, u8 b, u8 arg10) asm("func_80047214");
void GameDrawPolyLine3(void *ot, s16 x0, s16 y0, s16 x1, s16 y1, s16 x2, s16 y2, u8 r, u8 g, u8 b, u8 arg10) {
    u8 *prim;
    u8 *oldPrim;

    prim = *(u8 **)0x1F800000;
    func_80065034(prim);
    func_80064E90(prim, arg10 != 0xFF);

    *(s16 *)(prim + 0x8) = x0;
    *(s16 *)(prim + 0xA) = y0;
    *(s16 *)(prim + 0xC) = x1;
    *(s16 *)(prim + 0xE) = y1;
    *(s16 *)(prim + 0x10) = x2;
    *(s16 *)(prim + 0x12) = y2;
    prim[0x4] = r;
    prim[0x5] = g;
    prim[0x6] = b;

    oldPrim = prim;
    prim += 0x18;
    func_80064DDC(ot, oldPrim);

    if (arg10 != 0xFF) {
        prim = func_80017390(ot, prim, arg10);
    }

    *(u8 **)0x1F800000 = prim;
}

void func_80065020(u8 *arg0);
void SetSemiTrans(u8 *arg0, s32 enabled) asm("func_80064E90");
void *func_80017390(void *ot, void *prim, s32 arg2);

void GameDrawGradientLine(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r0, u8 g0, u8 b0, u8 r1, u8 g1, u8 b1, u8 alpha) asm("func_80047330");
void GameDrawGradientLine(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u8 r0, u8 g0, u8 b0, u8 r1, u8 g1, u8 b1, u8 alpha) {
    register s32 x0Reg asm("$17");
    register s32 y0Reg asm("$18");
    register s32 x1Reg asm("$19");
    register s32 y1Reg asm("$20");
    register s32 r0Reg asm("$21");
    register s32 g0Reg asm("$22");
    register s32 b0Reg asm("$23");
    register u8 alphaReg asm("$16");
    register u8 *a0Reg asm("$4");
    u8 *prim;
    u8 *oldPrim;
    u8 r1Local;
    u8 g1Local;
    u8 b1Local;

    prim = *(u8 **)0x1F800000;
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

    func_80065020(prim);
    a0Reg = prim;
    SetSemiTrans(a0Reg, alphaReg != 0xFF);

    *(s16 *)(prim + 0x8) = x0Reg;
    *(s16 *)(prim + 0xA) = y0Reg;
    *(s16 *)(prim + 0x10) = x1Reg;
    *(s16 *)(prim + 0x12) = y1Reg;
    prim[0x4] = r0Reg;
    prim[0x5] = g0Reg;
    prim[0x6] = b0Reg;
    prim[0xC] = r1Local;
    prim[0xD] = g1Local;
    prim[0xE] = b1Local;

    oldPrim = prim;
    prim += 0x14;
    AddPrim((u32 *)ot, (u32 *)oldPrim);

    if (alphaReg != 0xFF) {
        prim = func_80017390(ot, prim, alphaReg);
    }

    *(u8 **)0x1F800000 = prim;
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

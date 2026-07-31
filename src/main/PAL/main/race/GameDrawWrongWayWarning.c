#include "common.h"
#include "game/render.h"
#include "game/car.h"

u8 *GameDrawHudDigit(u8 *prim, s32 x, s32 y, s32 digit, u16 clut) asm("func_80033B7C");

void SetSprt(u8 *arg0) asm("func_80064FA8");
void SetShadeTex(u8 *arg0, s32 arg1) asm("func_80064EB8");
void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void *func_8001720C(void *ot, void *packet, s32 x, s32 y, s32 w, s32 h, s32 r, s32 g, s32 b);
void *func_80017390(void *ot, void *packet, s32 arg2);

void GameDrawWrongWayWarning(void) asm("func_800333DC");
void GameDrawWrongWayWarning(void) {
    /* This pin is load-bearing: removing it changes .text. */
    register u8 *packet __asm("$16");
    u8 *next;
    s32 i;
    s32 x;
    s32 u;
    u8 *ot;
    u8 *oldPacket;
    s32 temp;
    s32 uvOffset;
    u8 *ret;

    next = *(u8 **)0x1F800000;
    i = 0;
    u = 0x48;
    x = 0x6C;
    packet = next;

    do {
        SetSprt(next);
        SetShadeTex(next, 1);

        temp = 0x78;
        *(volatile s16 *)(packet + 0x0A) = temp;
        __asm__ volatile("" : : : "memory");
        uvOffset = (((i & 2) << 3) - (i & 2)) << 2;
        temp = -0x10 - uvOffset;
        uvOffset += 0x10;
        packet[0x0C] = temp;
        temp = 0x10;
        *(s16 *)(packet + 0x12) = temp;
        temp = 0x788C;
        oldPacket = packet;
        *(s16 *)(packet + 0x08) = x;
        packet[0x0D] = u;
        *(s16 *)(packet + 0x10) = uvOffset;
        *(s16 *)(packet + 0x0E) = temp;

        packet += 0x14;
        next += 0x14;
        u += 0x10;
        x += 0x10;
        ot = g_DrawBuffer;
        i++;
        AddPrim(ot + 0xCC, oldPacket);
    } while (i < 3);

    ret = func_8001720C(g_DrawBuffer + 0xCC, next, 0x64, 0x70, 0x78, 0x20, 8, 8, 8);
    *(void **)0x1F800000 = ret;
    *(void **)0x1F800000 = func_80017390(g_DrawBuffer + 0xCC, ret, 9);
}

#define SCRATCH (*(u8 **)0x1F800000)

extern u8 *volatile g_DrawBuffer asm("D_8019C900");
extern s32 g_PlayerSpeed asm("D_8009E778");
extern s16 g_PlayerGear asm("D_8009E806");
extern u16 g_HudGlyphClut asm("D_801E4130");
/* The needle corner table car/GameGetTrackSurfaceHeight.c rebuilds: four
 * (x, y) pairs. The loop below biases its cursor to the y of corner 0. */
extern s16 g_TachoNeedleQuad[4][2] asm("D_8019C7D4");
/* Tint of the tachometer face: the dial-mode branches set all three to the
 * same level and the tail of the function copies them into the face prim
 * already staged at g_DrawBuffer + 0x236E8. .data seeds them 0x80. */
extern u8 g_TachoFaceR asm("D_8007DF00");
extern u8 g_TachoFaceG asm("D_8007DF01");
extern u8 g_TachoFaceB asm("D_8007DF02");

s32 func_80068568(s32 angle);
s32 func_80068634(s32 angle);
void SetPolyF4(void *prim) asm("func_80064F30");
void func_80064FF8(void *prim);
void GameDrawSpeedDigits(s32 x, s32 y, s32 speed) asm("func_80033C18");

void GameDrawTachometer(s32 rpm, s32 arg1, s32 type, s32 amt) asm("func_8003351C");
void GameDrawTachometer(s32 rpm, s32 arg1, s32 type, s32 amt) {
    GameCarSpec *p = g_CarSpec;
    s32 cx = p->tachoNeedleX;
    s32 cy = p->tachoNeedleY;
    /* retail keeps a biased base register at &spec->tachoNeedleX; +28 is
     * spec->needleColor and +32 spec->needleColorAlt. */
    u8 *base = (u8 *)&p->tachoNeedleX;
    s32 b = p->needleAngleMin;
    s32 angle = b + rpm * (p->needleAngleMax - b) / 10000;
    s32 cos = func_80068568(angle);
    s32 sin = func_80068634(angle);
    u8 *prim = SCRATCH;
    s16 *vp;
    s16 *pa;
    s16 *pb;
    s32 i;
    u8 code7;

    SetPolyF4(prim);

    vp = (s16 *)(prim + 8);
    i = 0;
    pb = &g_TachoNeedleQuad[0][1];
    pa = pb - 1;
    for (; i < 4; i++) {
        *vp++ = cx + (sin * pa[0] - cos * pb[0]) / 4096;
        *vp++ = cy + (cos * pa[0] + sin * pb[0]) / 4096;
        pb += 2;
        pa += 2;
    }

    code7 = prim[7];

    if (type == 1) {
        if (amt > 96) amt = 96;
        g_TachoFaceB = -128 - amt;
        g_TachoFaceG = -128 - amt;
        g_TachoFaceR = -128 - amt;
        prim[4] = (amt * 32 + base[28] * (96 - amt)) / 96;
        prim[5] = (amt * 32 + base[29] * (96 - amt)) / 96;
        prim[6] = (amt * 32 + base[30] * (96 - amt)) / 96;
    } else if (type == 3) {
        amt -= 32;
        if (amt < 0) amt = 0;
        g_TachoFaceB = amt + 32;
        g_TachoFaceG = amt + 32;
        g_TachoFaceR = amt + 32;
        prim[4] = ((96 - amt) * 32 + base[28] * amt) / 96;
        prim[5] = ((96 - amt) * 32 + base[29] * amt) / 96;
        prim[6] = ((96 - amt) * 32 + base[30] * amt) / 96;
        *(s16 *)(g_DrawBuffer + 0x236F2) = 0x33A8;
    } else if (type == 2) {
        *(s16 *)(g_DrawBuffer + 0x236F2) = 0x33E8;
        g_TachoFaceB = 0x80;
        g_TachoFaceG = 0x80;
        g_TachoFaceR = 0x80;
        *(s32 *)(prim + 4) = *(s32 *)(base + 32);
    } else {
        s16 rv = 0x33A8;
        /* This barrier is load-bearing: removing it changes .text. */
        asm("" : "=r"(rv) : "0"(rv));
        *(s16 *)(g_DrawBuffer + 0x236F2) = rv;
        g_TachoFaceB = 0x80;
        g_TachoFaceG = 0x80;
        g_TachoFaceR = 0x80;
        *(s32 *)(prim + 4) = *(s32 *)(base + 28);
    }

    prim[7] = code7;
    AddPrim(g_DrawBuffer + 0xCC, prim);
    prim += 24;
    SCRATCH = prim;

    {
        s32 x = cx + *(s16 *)(base + 12);
        s32 y = cy + *(s16 *)(base + 14);
        u8 *q = GameDrawHudDigit(*(u8 *volatile *)0x1F800000, x, y, g_PlayerGear, g_HudGlyphClut);
        SCRATCH = q;
        GameDrawSpeedDigits(cx, cy, g_PlayerSpeed * 160 / 1168);
    }

    *(u8 *)(g_DrawBuffer + 0x236E8) = g_TachoFaceR;
    *(u8 *)(g_DrawBuffer + 0x236E9) = g_TachoFaceG;
    *(u8 *)(g_DrawBuffer + 0x236EA) = g_TachoFaceB;

    { u8 *g = g_DrawBuffer; AddPrim(g + 0xCC, g + 0x236CC); }
    { u8 *g = g_DrawBuffer; AddPrim(g + 0xCC, g + 0x236E4); }
    { u8 *g = g_DrawBuffer; AddPrim(g + 0xCC, g + 0x236D8); }

    {
        u8 *q = SCRATCH;
        u8 *g;
        u8 *prim;
        s32 v10;
        func_80064FF8(q);
        prim = q;
        *(s16 *)(q + 8) = cx + *(u16 *)(base + 16);
        v10 = cy + *(u16 *)(base + 18);
        *(s16 *)(q + 12) = 0x10;
        *(s16 *)(q + 14) = 0x10;
        q[4] = arg1 * 223 + 32;
        q[5] = 0x20;
        q[6] = 0x20;
        g = g_DrawBuffer;
        *(s16 *)(q + 10) = v10;
        q += 16;
        AddPrim(g + 0xCC, prim);
        SCRATCH = q;
    }
}

void SetSemiTrans(u8 *arg0, s32 arg1) asm("func_80064E90");
void SetTile(u8 *arg0) asm("func_80064FF8");
void *func_80017390(void *arg0, void *arg1, s32 arg2);

void GameDrawFullscreenFadeTile(s32 color, s32 arg1) asm("func_80033AA0");
void GameDrawFullscreenFadeTile(s32 color, s32 arg1) {
    u8 *base = g_DrawBuffer;
    u8 *ot = base + 0xCC;
    u8 *packet;
    u8 *next;
    u8 *prim;
    s32 height;

    if (color < 0) {
        color = 0;
    } else if (color >= 0x100) {
        color = 0xFF;
    }

    packet = *(u8 **)0x1F800000;
    SetTile(packet);
    SetSemiTrans(packet, 1);

    *(s16 *)(packet + 0xC) = 0x140;
    height = 0xF0;
    asm volatile("" : "=r"(prim) : "0"(packet));
    *(s16 *)(packet + 0x8) = 0;
    *(s16 *)(packet + 0xA) = 0;
    *(s16 *)(packet + 0xE) = height;
    packet[4] = color;
    packet[5] = color;
    packet[6] = color;

    asm volatile("" ::: "memory");
    next = packet + 0x10;
    asm volatile("" : : "r"(next));
    AddPrim((u32 *)ot, (u32 *)prim);
    *(void **)0x1F800000 = func_80017390(g_DrawBuffer + 0xCC, next, arg1);
}

void SetSprt8(u8 *prim) asm("func_80064F80");
void func_80064EB8(u8 *prim, s32 enabled);

u8 *GameDrawHudDigit(u8 *prim, s32 x, s32 y, s32 digit, u16 clut) asm("func_80033B7C");
u8 *GameDrawHudDigit(u8 *prim, s32 x, s32 y, s32 digit, u16 clut) {
    /* This pin is load-bearing: removing it changes .text. */
    register u8 *out asm("$16") = prim;
    s32 xReg = x;
    s32 yReg = y;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 codeReg asm("$17");
    register s32 arg4Reg asm("$20");

    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(xReg), "=r"(yReg) : "0"(xReg), "1"(yReg) : "$17");
    codeReg = digit;
    arg4Reg = clut;
    SetSprt8(out);
    func_80064EB8(out, 1);

    codeReg <<= 3;
    out[0xC] = codeReg;
    out[0xD] = 0x10;

    {
        u8 *ot = g_DrawBuffer;
        /* This pin is load-bearing: removing it changes .text. */
        register u8 *oldPrim asm("$5") = out;

        *(s16 *)&out[0x8] = xReg;
        *(s16 *)&out[0xA] = yReg;
        *(s16 *)&out[0xE] = arg4Reg;
        out += 0x10;
        AddPrim(ot + 0xCC, oldPrim);
    }

    return out;
}

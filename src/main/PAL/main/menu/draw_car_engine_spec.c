#include "common.h"
#include "game/menu.h"

extern s32 D_80082E70[];

void func_80016754(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void DrawEngineSpecLabel(s32 arg0, s32 arg1, s32 arg2) asm("func_80052128");
void DrawEngineSpecLabel(s32 arg0, s32 arg1, s32 arg2) {
    func_80016754(arg0, arg1, D_80082E70[arg2], 0x78CC);
}


typedef struct EngineSpecData {
    u8 pad00[0x10];
    s16 power;      /* 0x10 */
    s16 torque;     /* 0x12 */
    u8 unk14;       /* 0x14 */
    u8 unk15;       /* 0x15 */
    s16 unk16;      /* 0x16 */
} EngineSpecData;

extern EngineSpecData *g_EngineSpec asm("D_8009E698");
extern u8 D_80011B5C[];

void drawSprite(void *ot, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 r,
                s32 g, s32 b, s32 clut, s32 shadeTex, s32 semiTrans,
                s32 flags) asm("func_80046A2C");
void drawSmallText(s32 x, s32 y, u8 *str, s32 r, s32 g, s32 b, s32 clut,
                   s32 flags) asm("func_80047634");
s32 gsprintf(u8 *buf, u8 *fmt, s32 val) asm("func_800632F0");

void func_80052158(s32 a0raw, s32 arg1) {
    void *ot;
    u8 buf[0x40];
    s32 arg0;
    s32 n;
    s32 p;
    s32 q;
    s32 r;
    s32 torqueEnd;

    ot = (u8 *)*(void **)0x1F800004 + 4;
    if (g_MenuAltLayout != 0) {
        return;
    }
    arg0 = (s16)a0raw;
    drawSprite(ot, 0xA1, 0xCC - arg0, 0x10, 0xC, 0, 0xF4, arg1 & 0xFF, arg1 & 0xFF,
               arg1 & 0xFF, 0x244, 0, 1, 0x3A);
    drawSprite(ot, 0xB2, 0xCC - arg0, 0x1C, 0xC, 0x10, 0xF4, arg1 & 0xFF, arg1 & 0xFF,
               arg1 & 0xFF, 0x244, 0, 1, 0x3A);
    n = gsprintf(buf, D_80011B5C, g_EngineSpec->power);
    drawSmallText(0xD2, 0xCB - arg0, buf, arg1 & 0xFF, arg1 & 0xFF, arg1 & 0xFF, 0x244,
                  0x20);
    p = n * 6;
    drawSprite(ot, p + 0xD4, 0xCC - arg0, 8, 0xC, 0x70, 0xF4, arg1 & 0xFF, arg1 & 0xFF,
               arg1 & 0xFF, 0x244, 0, 1, 0x3A);
    drawSprite(ot, p + 0xDF, 0xCC - arg0, 6, 0xC, 0xD8, 0, arg1 & 0xFF, arg1 & 0xFF,
               arg1 & 0xFF, 0x244, 0, 1, 0x3B);
    q = p + 0xE6;
    n = gsprintf(buf, D_80011B5C, g_EngineSpec->torque);
    drawSmallText(q, 0xCB - arg0, buf, arg1 & 0xFF, arg1 & 0xFF, arg1 & 0xFF, 0x244,
                  0x20);
    torqueEnd = p + 0xE8;
    drawSprite(ot, torqueEnd + (n * 6), 0xCC - arg0, 0x10, 0xC, 0x78, 0xF4, arg1 & 0xFF,
               arg1 & 0xFF, arg1 & 0xFF, 0x244, 0, 1, 0x3A);

    drawSprite(ot, 0xA1, 0xDA - arg0, 0x10, 0xC, 0, 0xF4, arg1 & 0xFF, arg1 & 0xFF,
               arg1 & 0xFF, 0x244, 0, 1, 0x3A);
    drawSprite(ot, 0xB2, 0xDA - arg0, 0x20, 0xC, 0x2C, 0xF4, arg1 & 0xFF, arg1 & 0xFF,
               arg1 & 0xFF, 0x244, 0, 1, 0x3A);
    n = gsprintf(buf, D_80011B5C, g_EngineSpec->unk15);
    drawSmallText(0xD2, 0xD9 - arg0, buf, arg1 & 0xFF, arg1 & 0xFF, arg1 & 0xFF, 0x244,
                  0x20);
    p = n * 6;
    drawSprite(ot, p + 0xD3, 0xDA - arg0, 3, 0xC, 0xE0, 0, arg1 & 0xFF, arg1 & 0xFF,
               arg1 & 0xFF, 0x244, 0, 1, 0x3B);
    q = p + 0xD5;
    n = gsprintf(buf, D_80011B5C, g_EngineSpec->unk14);
    drawSmallText(q, 0xD9 - arg0, buf, arg1 & 0xFF, arg1 & 0xFF, arg1 & 0xFF, 0x244,
                  0x20);
    p += 0xD7;
    /* Written as an operand swap: the target keeps the string length in
       the higher callee-saved than the running cursor. */
    q = n;
    n = p;
    q = n + (q * 6);
    drawSprite(ot, q, 0xDA - arg0, 0x10, 0xC, 0x88, 0xF4, arg1 & 0xFF, arg1 & 0xFF,
               arg1 & 0xFF, 0x244, 0, 1, 0x3A);
    drawSprite(ot, q + 0x11, 0xDA - arg0, 6, 0xC, 0xD8, 0, arg1 & 0xFF, arg1 & 0xFF,
               arg1 & 0xFF, 0x244, 0, 1, 0x3B);
    q += 0x18;
    n = gsprintf(buf, D_80011B5C, g_EngineSpec->unk16);
    drawSmallText(q, 0xD9 - arg0, buf, arg1 & 0xFF, arg1 & 0xFF, arg1 & 0xFF, 0x244,
                  0x20);
    r = q + 2;
    drawSprite(ot, r + (n * 6), 0xDA - arg0, 0x10, 0xC, 0x78, 0xF4, arg1 & 0xFF,
               arg1 & 0xFF, arg1 & 0xFF, 0x244, 0, 1, 0x3A);
}


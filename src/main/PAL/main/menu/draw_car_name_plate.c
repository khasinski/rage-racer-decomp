#include "common.h"
#include "game/car.h"
#include "game/menu.h"
#include "game/scratchpad.h"

void GameDrawSpriteWide(void *ot, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 r,
                   s32 g, s32 b, s32 clut, s32 shadeTex, s32 semiTrans,
                   s32 flags) asm("DrawSprite");
/* The bottom-right plate: grade digit, manufacturer sprite and model-name sprite. */
void DrawCarNamePlate(s32 arg0, s32 arg1, s32 arg2) {
    void *ot;
    u32 idx;
    u32 shade;

    ot = (u8 *)SCRATCH_OT_BASE_AS(void) + 4;
    if (arg0 == 0) {
        g_CarNamePlateFade = 0;
        return;
    }
    if (arg0 < 0) {
        g_CarNamePlateFade += arg0;
        if (g_CarNamePlateFade < 0) {
            g_CarNamePlateFade = 0;
        }
    }

    shade = (u32)g_CarNamePlateFade / 4;
    GameDrawSpriteWide(ot, 0x100, 0x168, 0x20, 0x10, 0x7C, 0x7C, (u8)shade,
                  (u8)shade, (u8)shade, 0x244, 0, 1, 0x3B);

    idx = (GetCarUnlockLevel(arg1) + arg2) & 0xFFFF;
    if (idx >= 5) {
        GameDrawSpriteWide(ot, 0x11F, 0x168, 8, 0x10, 0x38, 0x28, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3B);
    } else {
        GameDrawSpriteWide(ot, 0x11F, 0x168, 8, 0x10, (s16)((idx * 8) + 8), 0x18,
                      (u8)shade, (u8)shade, (u8)shade, 0x244, 0, 1,
                      0x3B);
    }

    switch (arg1) {
    case 0:
    case 1:
    case 2:
    case 10:
        GameDrawSpriteWide(ot, 0x112, 0x178, 0x14, 0x10, 0x50, 0xBC, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3B);
        break;
    case 3:
        GameDrawSpriteWide(ot, 0x105, 0x178, 0x20, 0x10, 0, 0xBC, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3B);
        break;
    case 4:
    case 5:
    case 6:
    case 11:
        GameDrawSpriteWide(ot, 0x106, 0x178, 0x20, 0x10, 0x64, 0xBC, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3B);
        break;
    case 7:
    case 8:
    case 9:
    case 12:
        GameDrawSpriteWide(ot, 0xF6, 0x178, 0x30, 0x10, 0x20, 0xBC, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3B);
        break;
    default:
        break;
    }

    switch (arg1) {
    case 0:
        GameDrawSpriteWide(ot, 0xFC, 0x188, 0x2A, 0x10, 0xA, 0x30, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    case 1:
        GameDrawSpriteWide(ot, 0x106, 0x188, 0x20, 0x10, 0x48, 0x30, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    case 2:
        GameDrawSpriteWide(ot, 0x106, 0x188, 0x20, 0x10, 0x7C, 0x30, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    case 10:
        GameDrawSpriteWide(ot, 0xFA, 0x188, 0x2C, 0x10, 0xA4, 0x30, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    case 3:
        GameDrawSpriteWide(ot, 0xF2, 0x188, 0x34, 0x10, 0, 0x40, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    case 4:
        GameDrawSpriteWide(ot, 0xFD, 0x188, 0x28, 0x10, 0x74, 0x50, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    case 5:
        GameDrawSpriteWide(ot, 0xFC, 0x188, 0x2A, 0x10, 0x3E, 0x50, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    case 6:
        GameDrawSpriteWide(ot, 0x107, 0x188, 0x20, 0x10, 0xB0, 0x50, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    case 11:
        GameDrawSpriteWide(ot, 0xFC, 0x188, 0x2A, 0x10, 0xA, 0x60, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    case 7:
        GameDrawSpriteWide(ot, 0xFE, 0x188, 0x28, 0x10, 0x40, 0x40, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    case 8:
        GameDrawSpriteWide(ot, 0x104, 0x188, 0x22, 0x10, 0x7A, 0x40, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    case 9:
        GameDrawSpriteWide(ot, 0xF7, 0x188, 0x30, 0x10, 0xA0, 0x40, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    case 12:
        GameDrawSpriteWide(ot, 0xF6, 0x188, 0x30, 0x10, 4, 0x50, (u8)shade,
                      (u8)shade, (u8)shade, 0x244, 0, 1, 0x3E);
        break;
    }

    if (arg0 > 0) {
        g_CarNamePlateFade += arg0;
        if (g_CarNamePlateFade >= 509) {
            g_CarNamePlateFade = 508;
        }
    }
}

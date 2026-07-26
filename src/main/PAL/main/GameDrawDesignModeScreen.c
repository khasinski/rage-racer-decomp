#include "common.h"

typedef struct {
    u8 cells[6][6];
} CellMask;

extern CellMask D_80011BD4;
extern s32 D_8009B2D4;

void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clutX, s32 shadeTex,
                   s32 semiTrans, s32 flags);

s32 GameDrawDesignModeScreen(s32 step) asm("func_80056E64");
s32 GameDrawDesignModeScreen(s32 arg0) {
    CellMask mask;
    void *ot;
    s32 limit;
    s32 offset;
    s32 intensity;
    s32 y;
    s32 row;
    s32 column;

    ot = (void *)(*(u32 *)0x1F800004 + 4);
    mask = D_80011BD4;

    if (arg0 == 0) {
        D_8009B2D4 = 0;
        return;
    }

    if (arg0 > 0) {
        s32 updated;

        updated = D_8009B2D4 + arg0;
        D_8009B2D4 = updated;
        if (updated >= 0x1FD) {
            D_8009B2D4 = 0x1FC;
        }
        offset = 0;
    } else {
        s32 updated;

        updated = D_8009B2D4 + arg0;
        D_8009B2D4 = updated;
        if (updated < 0) {
            D_8009B2D4 = 0;
        }
        limit = 0x1FC - D_8009B2D4;
        offset = (u32)(limit * limit) >> 11;
    }

    y = 0xB0 - (s16)offset;
    intensity = (u32)D_8009B2D4 >> 2;

    func_80046A2C(ot, 0xB4, y, 0x18, 0xC, 0x94, 0xDC,
                  intensity & 0xFF, intensity & 0xFF, intensity & 0xFF,
                  0x244, 0, 1, 0x3B);
    func_80046A2C(ot, 0xCE, y, 0x14, 0xC, 0xE0, 0xDC,
                  intensity & 0xFF, intensity & 0xFF, intensity & 0xFF,
                  0x244, 0, 1, 0x3B);

    for (row = 0; row < 6; row++) {
        for (column = 0; column < 6; column++) {
            s32 clutX;

            if (mask.cells[row][column] != 0) {
                clutX = 0x26F;
            } else {
                clutX = 0x244;
            }

            func_80046A2C(ot, 0xB4 + column * 0x10,
                          0xC0 + row * 0x20 - (s16)offset,
                          0xC, 0x18, 0xF4, 0x60,
                          intensity & 0xFF, intensity & 0xFF,
                          intensity & 0xFF,
                          clutX, 0, 1, 0x39);
        }
    }

    return D_8009B2D4;
}

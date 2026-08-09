#include "common.h"
#include "game/memcard.h"
#include "psyq/gpu.h"
#include "game/menu.h"

void ClearSaveHeaderRows(GameSaveHeaderRow *rows) {
    u8 *rowBytes = (u8 *)rows;
    s32 i = 0;
    u8 *ptr1 = rowBytes;
    s32 j;
    u8 *ptr2;
    u8 *ptr3;

    do {
        rowBytes[0] = 0;
        j = 5;
        ptr2 = rowBytes + 5;
        do {
            ptr2[1] = 0;
            ptr2--;
            j--;
        } while (j >= 0);

        *(volatile u32 *)&rowBytes[8] = 0;

        j = 0;
        ptr3 = ptr1;
        do {
            j++;
        } while ((*(u16 *)&ptr3[0xC] = 0, j < 0x38));

        *(volatile u32 *)&rowBytes[0x7C] = 0;
        rowBytes += 0x80;
        i++;
        ptr1 += 0x82;
    } while (i < 3);
}

/* sprintf: every caller declares its own arity; keep it prototypeless. */

/* The icon strip is copied to VRAM in two passes: one 16x1 run of palette
 * entries at block+0x60, then the 4x16 frame tiles from block+0x80 on.  The
 * frame loop is written against the counter rather than against running
 * offsets because that is what the retail code's induction variables are:
 * `i * 0x80` and `i * 4` are strength-reduced back into the `addiu s4,s4,128`
 * / `addiu s1,s1,4` pair, and doing it this way puts the two increments'
 * initialisers after the loop-invariant `4` and `0x10` in the preheader,
 * which is the order retail schedules them in. */
void BuildSaveIconBlock(u8 *block, char *title, s32 iconTile, s32 imageX, s32 imageY) {
    Rect *iconRect;
    Rect *frameRect;
    s32 i;
    s32 tileRow;
    s32 tileX;

    block[0] = 'S';
    block[1] = 'C';
    block[2] = 0x11;
    block[3] = 1;
    sprintf(block + 4, g_FmtString, title);

    tileRow = iconTile / 20;
    iconRect = &g_SaveIconRect;
    g_SaveIconRect.w = 0x10;
    g_SaveIconRect.h = 1;
    tileX = iconTile % 20;
    iconRect->x = tileX * 16;
    g_SaveIconRect.y = tileRow + 0x1E0;
    StoreImage(iconRect, block + 0x60);
    DrawSync(0);

    i = 0;
    frameRect = iconRect;
    do {
        frameRect->x = imageX + i * 4;
        frameRect->y = imageY;
        frameRect->w = 4;
        frameRect->h = 0x10;
        StoreImage(frameRect, block + 0x80 + i * 0x80);
        DrawSync(0);
        i++;
    } while (i <= 0);
}

void WriteSaveHeaderRow(GameSaveHeaderRow *row) {
    u8 *rowBytes = row->bytes;
    s32 i;
    u32 checksum;
    u16 *scan;

    rowBytes[0] = g_TeamNameLength;

    for (i = 0; i < 7; i++) {
        *((rowBytes + i) + 1) = g_TeamNameChars[i];
    }

    i = 0;
    checksum = 0;
    *(s32 *)(rowBytes + 8) = g_SaveElapsedTicks;
    scan = (u16 *)rowBytes;

    do {
        checksum += *scan++;
        i++;
    } while ((u32)i < 0x3E);

    checksum = ~checksum;
    *(u32 *)(rowBytes + 0x7C) = checksum;
}

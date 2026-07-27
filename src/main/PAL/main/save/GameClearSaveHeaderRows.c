#include "common.h"
#include "game/memcard.h"
#include "psyq/gpu.h"
#include "game/menu.h"

void GameClearSaveHeaderRows(GameSaveHeaderRow *rows) {
    u8 *arg0 = (u8 *)rows;
    s32 i = 0;
    u8 *ptr1 = arg0;
    s32 j;
    u8 *ptr2;
    u8 *ptr3;

    do {
        arg0[0] = 0;
        j = 5;
        ptr2 = arg0 + 5;
        do {
            ptr2[1] = 0;
            ptr2--;
            j--;
        } while (j >= 0);

        *(volatile u32 *)&arg0[8] = 0;

        j = 0;
        ptr3 = ptr1;
        do {
            j++;
        } while ((*(u16 *)&ptr3[0xC] = 0, j < 0x38));

        *(volatile u32 *)&arg0[0x7C] = 0;
        arg0 += 0x80;
        i++;
        ptr1 += 0x82;
    } while (i < 3);
}

extern char g_FmtString[] asm("D_80012F8C");
extern Rect g_SaveIconRect asm("D_8009B55C");

void LibcSprintf(void *dst, char *fmt, s32 arg0) asm("func_800632F0");
void StoreImage(Rect *rect, void *data) asm("func_80065B88");
void DrawSync(long mode) asm("func_800658FC");

void GameBuildSaveIconBlock(u8 *block, char *title, s32 iconTile, s32 imageX, s32 imageY) {
    register u8 *blockReg asm("$18");
    register char *titleReg asm("$3");
    register s32 iconTileReg asm("$16");
    register s32 imageXReg asm("$17");
    register Rect *rectArg asm("$4");
    register u8 *imageData asm("$5");
    register Rect *rect asm("$19");
    register s32 dataOffset asm("$20");
    register s32 i asm("$21");
    register s32 rectW asm("$23");
    register s32 rectH asm("$22");
    register s32 magic asm("$2");
    register s32 tileRow asm("$3");
    register s32 sign asm("$2");
    s32 tileX;

    blockReg = block;
    asm("" : "=r"(blockReg) : "0"(blockReg));
    titleReg = title;
    asm("" : "=r"(titleReg) : "0"(titleReg));
    iconTileReg = iconTile;
    asm("" : "=r"(iconTileReg) : "0"(iconTileReg));
    imageXReg = imageX;
    asm("" : "=r"(imageXReg) : "0"(imageXReg));

    blockReg[0] = 'S';
    blockReg[1] = 'C';
    blockReg[2] = 0x11;
    blockReg[3] = 1;
    LibcSprintf(blockReg + 4, g_FmtString, (s32)titleReg);

    magic = 0x66666667;
    asm("" : "=r"(magic) : "0"(magic));
    rectArg = &g_SaveIconRect;
    asm("" : "=r"(rectArg) : "0"(rectArg));
    asm("mult %0,%1" : : "r"(iconTileReg), "r"(magic));
    imageData = blockReg + 0x60;
    i = 0;
    rect = rectArg;
    asm("" : "=r"(rect) : "0"(rect));
    rectW = 4;
    asm("" : "=r"(rectW) : "0"(rectW));
    rectH = 0x10;
    asm("" : "=r"(rectH) : "0"(rectH));
    g_SaveIconRect.w = 0x10;
    g_SaveIconRect.h = 1;
    asm("" ::: "memory");
    sign = iconTileReg >> 31;
    asm("mfhi %0" : "=r"(tileRow));
    tileRow >>= 3;
    tileRow -= sign;
    tileX = iconTileReg - (tileRow * 20);
    rectArg->x = tileX << 4;
    g_SaveIconRect.y = tileRow + 0x1E0;
    StoreImage(rectArg, imageData);
    dataOffset = 0x80;
    DrawSync(0);

    do {
        rect->x = imageXReg;
        rect->y = imageY;
        rect->w = rectW;
        rect->h = rectH;
        StoreImage(rect, blockReg + dataOffset);
        DrawSync(0);
        dataOffset += 0x80;
        i++;
        imageXReg += 4;
    } while (i <= 0);
}

extern s32 g_SaveElapsedTicks asm("D_801E7A54");

void GameWriteSaveHeaderRow(GameSaveHeaderRow *row) {
    u8 *arg0 = (u8 *)row;
    register s32 i asm("$5");
    register u32 checksum asm("$3");
    register u16 *scan asm("$6");

    arg0[0] = g_TeamNameLength;

    for (i = 0; i < 7; i++) {
        *((arg0 + i) + 1) = g_TeamNameChars[i];
    }

    i = 0;
    checksum = 0;
    *(s32 *)(arg0 + 8) = g_SaveElapsedTicks;
    scan = (u16 *)arg0;

    do {
        checksum += *scan++;
        i++;
    } while ((u32)i < 0x3E);

    *(u32 *)(arg0 + 0x7C) = ~checksum;
}

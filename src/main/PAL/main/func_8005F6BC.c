#include "game/memcard.h"
#include "psyq/gpu.h"

extern char D_80012F8C[];
extern Rect D_8009B55C;

void Square_Vsprintf(void *dst, char *fmt, s32 arg0) asm("func_800632F0");
void StoreImage(Rect *rect, void *data) asm("func_80065B88");
void DrawSync(s32 mode) asm("func_800658FC");

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
    Square_Vsprintf(blockReg + 4, D_80012F8C, (s32)titleReg);

    magic = 0x66666667;
    asm("" : "=r"(magic) : "0"(magic));
    rectArg = &D_8009B55C;
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
    D_8009B55C.w = 0x10;
    D_8009B55C.h = 1;
    asm("" ::: "memory");
    sign = iconTileReg >> 31;
    asm("mfhi %0" : "=r"(tileRow));
    tileRow >>= 3;
    tileRow -= sign;
    tileX = iconTileReg - (tileRow * 20);
    rectArg->x = tileX << 4;
    D_8009B55C.y = tileRow + 0x1E0;
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

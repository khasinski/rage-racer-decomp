#include "common.h"
#include "game/memcard.h"

extern u8 D_8007F45C;
extern u8 D_8007F460[];
extern s32 D_801E7A54;

void GameWriteSaveHeaderRow(GameSaveHeaderRow *row) {
    u8 *arg0 = (u8 *)row;
    register s32 i asm("$5");
    register u32 checksum asm("$3");
    register u16 *scan asm("$6");

    arg0[0] = D_8007F45C;

    for (i = 0; i < 7; i++) {
        *((arg0 + i) + 1) = D_8007F460[i];
    }

    i = 0;
    checksum = 0;
    *(s32 *)(arg0 + 8) = D_801E7A54;
    scan = (u16 *)arg0;

    do {
        checksum += *scan++;
        i++;
    } while ((u32)i < 0x3E);

    *(u32 *)(arg0 + 0x7C) = ~checksum;
}

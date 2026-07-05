#include "common.h"
#include "game/memcard.h"

void GameClearSaveHeaderRows(GameSaveHeaderRow *rows) {
    u8 *arg0 = (u8 *)rows;
    register s32 i asm("a3") = 0;
    register u8 *ptr1 asm("a2") = arg0;
    register s32 j asm("v1");
    register u8 *ptr2 asm("v0");
    register u8 *ptr3 asm("a1");

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

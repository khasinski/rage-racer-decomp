#include "common.h"

extern u32 g_TeamLogoCanvas[] asm("D_801E6F2C");

void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void GameScrollTeamLogoRight(void) asm("func_8004BBA8");
void GameScrollTeamLogoRight(void) {
    register s32 row asm("$7");
    register u32 *savePtr asm("$4");
    register u32 *savePtr2 asm("$10");
    register u32 *rowBase asm("$8");
    register s32 offset asm("$9");
    register s32 col asm("$6");
    register u32 *base asm("$11");
    register u32 *base2 asm("$5");
    register u32 *addr asm("$2");
    register u32 *cursor asm("$3");
    u32 saved[64];
    register u32 savedNibble asm("$2");
    register u32 value asm("$3");
    register u32 prev asm("$4");

    GamePlaySoundCue(1);

    row = 0;
    savePtr = saved;
    base = g_TeamLogoCanvas;
    cursor = base;
    do {
        savedNibble = cursor[7];
        cursor += 8;
        row++;
        savedNibble >>= 28;
        *savePtr = savedNibble;
        savePtr++;
    } while (row < 0x40);

    row = 0;
    rowBase = base;
    savePtr2 = saved;
    offset = 0;
    do {
        col = 7;
        base2 = base + 7;
        do {
            addr = (u32 *)(offset + (s32)base2);
            base2--;
            col--;
            value = addr[0];
            prev = addr[-1];
            value <<= 4;
            prev >>= 28;
            value |= prev;
            addr[0] = value;
        } while (col > 0);

        savedNibble = *savePtr2;
        savePtr2++;
        offset += 0x20;
        value = rowBase[0];
        row++;
        value <<= 4;
        value |= savedNibble;
        rowBase[0] = value;
        rowBase += 8;
    } while (row < 0x40);
}

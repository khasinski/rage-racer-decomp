#include "common.h"

extern u32 g_TeamLogoCanvas[] asm("D_801E6F2C");

void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void GameFlipTeamLogoVertical(void) asm("func_8004BC68");
void GameFlipTeamLogoVertical(void) {
    s32 i;
    s32 j;
    s32 mirror;
    u32 *base;

    GamePlaySoundCue(8);
    base = g_TeamLogoCanvas;
    i = 0;
    mirror = 0x3F;
    do {
        u8 *cursor;
        s32 leftOffset;
        s32 rightOffset;

        j = 0;
        leftOffset = i << 5;
        rightOffset = (mirror - i) << 5;
        cursor = (u8 *)base;
        do {
            u32 temp;
            u32 *left;
            u32 *right;

            left = (u32 *)(leftOffset + (s32)cursor);
            right = (u32 *)(rightOffset + (s32)cursor);
            cursor += 4;
            temp = *left;
            *left = *right;
            j++;
            *right = temp;
        } while (j < 8);
        i++;
    } while (i < 0x20);
}

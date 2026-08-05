#include "common.h"
#include "game/race.h"

extern s16 g_PlayerLap asm("D_8009E83C");
extern s32 g_LapCount asm("D_801E4364");
extern u8 g_BestTotalTimes[] asm("D_8019C70C");

void DrawTimeValue(s32 x, s32 y, s32 value, s32 color, s32 divisor) asm("func_80033D50");
void DrawSplitDelta(s32 arg0, s32 arg1);

void DrawSplitTimes(void);
void DrawSplitTimes(void) {
    s32 value;
    s32 tile;
    s32 timeout;
    s32 threshold;
    s32 finalValue;

    switch (0) { default:
    if (g_SplitTimer >= 0x3C) {
        threshold = 0x927BE;
        value = g_LapTimeMs;

    } else if (g_SectorIndex >= 0) {
        if (g_SplitSign != 0) {
            if (g_LapCount >= g_PlayerLap) {
                value = g_SplitDelta;
                if (g_SplitSign > 0) {
                    tile = 0x7810;
                } else {
                    tile = 0x780F;
                }
                DrawTimeValue(0x80, 0x50, value, tile, 0x3E8);
            }
        }
        threshold = 0x927BE;
        value = g_LastSectorTime;
    } else {
        break;
    }

    if (value <= threshold) {
        tile = 0x78CC;
    } else {
        tile = 0x7890;
    }
    DrawTimeValue(0x12, 0x2A, value, tile, 0x3E8);

    }
    timeout = 0x3E8;
    DrawTimeValue(0x12, 0x20, g_SplitTargetTime, 0x78CC, timeout);
    DrawSplitDelta(g_SplitSector, g_SplitSign);

    {
        s32 finalA0 = 0xFA;
        s32 finalA1 = 0x7C;
        s32 finalA3 = 0x78CC;

        finalValue = *(s32 *)&g_BestTotalTimes[(g_CourseIndex << 3) + (g_RaceSeries << 5)];
        DrawTimeValue(finalA0, finalA1, finalValue, finalA3, timeout);
    }
}

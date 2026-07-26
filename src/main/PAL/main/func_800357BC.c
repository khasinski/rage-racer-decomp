#include "common.h"
#include "game/race.h"

extern s16 D_8009AFA8;
extern s16 D_8009AFAC;
extern s16 g_PlayerLap asm("D_8009E83C");
extern s16 D_8009AFA4;
extern s32 D_8009AF7C;
extern s32 D_8009AF78;
extern s32 D_8009AFB0;
extern s32 D_801E4D64;
extern s32 D_801E4148;
extern s32 g_LapCount asm("D_801E4364");
extern u8 g_BestTotalTimes[] asm("D_8019C70C");

void func_80033D50(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80033308(s32 arg0, s32 arg1);

void func_800357BC(void) {
    s32 value;
    s32 tile;
    s32 timeout;
    s32 threshold;
    s32 finalValue;

    if (D_8009AFA8 >= 0x3C) {
        threshold = 0x927BE;
        value = D_801E4D64;
        goto compare_first;
    } else if (D_801E4148 >= 0) {
        if (D_8009AFAC != 0) {
            if (g_LapCount >= g_PlayerLap) {
                value = D_8009AF7C;
                if (D_8009AFAC > 0) {
                    tile = 0x7810;
                } else {
                    tile = 0x780F;
                }
                func_80033D50(0x80, 0x50, value, tile, 0x3E8);
            }
        }
        threshold = 0x927BE;
        value = D_8009AF78;
    } else {
        goto skip_first;
    }

compare_first:
    if (value <= threshold) {
        tile = 0x78CC;
    } else {
        tile = 0x7890;
    }
    func_80033D50(0x12, 0x2A, value, tile, 0x3E8);

skip_first:
    timeout = 0x3E8;
    func_80033D50(0x12, 0x20, D_8009AFB0, 0x78CC, timeout);
    func_80033308(D_8009AFA4, D_8009AFAC);

    {
        s32 finalA0 = 0xFA;
        s32 finalA1 = 0x7C;
        s32 finalA3 = 0x78CC;

        finalValue = *(s32 *)&g_BestTotalTimes[(g_CourseIndex << 3) + (g_RaceSeries << 5)];
        func_80033D50(finalA0, finalA1, finalValue, finalA3, timeout);
    }
}

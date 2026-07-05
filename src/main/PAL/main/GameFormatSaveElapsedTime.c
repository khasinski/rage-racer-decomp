#include "common.h"
#include "game/memcard.h"

extern char D_80012FB8[];

void Square_Vsprintf(void *arg0, char *arg1, s32 arg2, s32 arg3, s32 arg4) asm("func_800632F0");

void *GameFormatSaveElapsedTime(void *arg0, u32 arg1) {
    u32 hours = arg1 / 216000;
    u32 totalMinutes = arg1 / 3600;
    u32 totalSeconds = arg1 / 60;

    Square_Vsprintf(arg0, D_80012FB8, hours, totalMinutes - (hours * 60), totalSeconds - (totalMinutes * 60));
    return (u8 *)arg0 + 2;
}

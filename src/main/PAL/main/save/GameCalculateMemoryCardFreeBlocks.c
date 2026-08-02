#include "common.h"
#include "game/memcard.h"
#include "game/menu.h"

extern char g_McDirEntries[] asm("D_8009B748");

s32 GameCalculateMemoryCardFreeBlocks(s32 arg0) {
    u8 scratch[8];
    s32 i;
    s32 sum;
    u8 *ptr;
    s32 value;

    i = 0;
    sum = 0;

    if (arg0 > 0) {
        ptr = (u8 *)g_McDirEntries;
        do {
            value = *(s32 *)(ptr + 0x18);
            sum += value;
            ptr += 0x28;
        } while (++i < arg0);
    }

    {
        s32 biased;

        biased = sum;
        if (sum < 0) {
            biased = sum + 0x1FFF;
        }
        sum = biased >> 13;

        return 0xF - sum;
    }
}

extern s32 g_McCardFileCount asm("D_8009B738");
extern s32 g_McFreeBlocks asm("D_8009B73C");

s32 GameRefreshMemoryCardSaveStatus(s32 arg0, GameSaveHeaderRow *arg1) {
    s32 ret;

    GameMenuLoadPhase = 0x100;
    GameClearSaveHeaderRows(arg1);
    g_McCardFileCount = GameCountMemoryCardFiles(0, 0);
    g_McFreeBlocks = GameCalculateMemoryCardFreeBlocks(g_McCardFileCount);
    ret = GameScanMemoryCardSaveHeaders(arg1);
    GameMenuLoadPhase = 0x200;

    return ret;
}

extern char g_FmtPlayTime[] asm("D_80012FB8");

/* sprintf: every caller declares its own arity; keep it prototypeless. */
void LibcSprintf() asm("func_800632F0");

void *GameFormatSaveElapsedTime(void *arg0, u32 arg1) {
    u32 hours = arg1 / 216000;
    u32 totalMinutes = arg1 / 3600;
    u32 totalSeconds = arg1 / 60;

    LibcSprintf(arg0, g_FmtPlayTime, hours, totalMinutes - (hours * 60), totalSeconds - (totalMinutes * 60));
    return (u8 *)arg0 + 2;
}

extern u8 g_McMessageText[] asm("D_800128FC");

void func_80016754(s32 arg0, s32 arg1, void *arg2, s32 arg3);

void GameDrawMemoryCardMessageLine(s32 arg0, s32 arg1) {
    func_80016754(0x28, 0xB8, &g_McMessageText[arg1 * 30], 0x78CC);
}

extern u8 g_McHelpText[] asm("D_80012ADC");

void func_80016754(s32, s32, void *, s32);

void GameDrawMemoryCardHelpPrompt(s32 arg0) {
    s32 offset;
    u8 *base;
    s32 x;
    s32 y;

    offset = arg0 << 4;
    offset -= arg0;
    x = 0x50;
    y = 0x28;
    __asm__("" : "=r"(x), "=r"(y) : "0"(x), "1"(y));
    offset <<= 2;
    base = g_McHelpText;
    func_80016754(x, y, base + offset, 0x78CC);

    x = 0x50;
    y = 0x40;
    __asm__("" : "=r"(x), "=r"(y) : "0"(x), "1"(y));
    base += 0x1E;
    func_80016754(x, y, base + offset, 0x78CC);
}

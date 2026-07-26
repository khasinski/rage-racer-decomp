#include "common.h"
#include "game/memcard.h"
#include "game/menu.h"

extern s32 D_8009B748[];

s32 GameCalculateMemoryCardFreeBlocks(s32 arg0) {
    u8 scratch[8];
    s32 i;
    s32 sum;
    u8 *ptr;
    s32 value;

    i = 0;
    sum = 0;

    if (arg0 > 0) {
        ptr = (u8 *)D_8009B748;
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

extern s32 D_8009B738;
extern s32 D_8009B73C;

s32 GameRefreshMemoryCardSaveStatus(s32 arg0, GameSaveHeaderRow *arg1) {
    s32 ret;

    GameMenuLoadPhase = 0x100;
    GameClearSaveHeaderRows(arg1);
    D_8009B738 = GameCountMemoryCardFiles(0, 0);
    D_8009B73C = GameCalculateMemoryCardFreeBlocks(D_8009B738);
    ret = GameScanMemoryCardSaveHeaders(arg1);
    GameMenuLoadPhase = 0x200;

    return ret;
}

extern char D_80012FB8[];

void func_800632F0(void *arg0, char *arg1, s32 arg2, s32 arg3, s32 arg4);

void *GameFormatSaveElapsedTime(void *arg0, u32 arg1) {
    u32 hours = arg1 / 216000;
    u32 totalMinutes = arg1 / 3600;
    u32 totalSeconds = arg1 / 60;

    func_800632F0(arg0, D_80012FB8, hours, totalMinutes - (hours * 60), totalSeconds - (totalMinutes * 60));
    return (u8 *)arg0 + 2;
}

extern u8 D_800128FC[];

void func_80016754(s32 arg0, s32 arg1, void *arg2, s32 arg3);

void GameDrawMemoryCardMessageLine(s32 arg0, s32 arg1) {
    func_80016754(0x28, 0xB8, &D_800128FC[arg1 * 30], 0x78CC);
}

extern u8 D_80012ADC[];

void func_80016754(s32, s32, void *, s32);

void GameDrawMemoryCardHelpPrompt(s32 arg0) {
    register s32 offset asm("$17");
    register u8 *base asm("$16");
    register s32 x asm("$4");
    register s32 y asm("$5");

    offset = arg0 << 4;
    offset -= arg0;
    x = 0x50;
    y = 0x28;
    __asm__("" : "=r"(x), "=r"(y) : "0"(x), "1"(y));
    offset <<= 2;
    base = D_80012ADC;
    func_80016754(x, y, base + offset, 0x78CC);

    x = 0x50;
    y = 0x40;
    __asm__("" : "=r"(x), "=r"(y) : "0"(x), "1"(y));
    base += 0x1E;
    func_80016754(x, y, base + offset, 0x78CC);
}

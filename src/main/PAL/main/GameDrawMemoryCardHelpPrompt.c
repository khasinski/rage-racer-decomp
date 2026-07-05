#include "common.h"
#include "game/memcard.h"

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

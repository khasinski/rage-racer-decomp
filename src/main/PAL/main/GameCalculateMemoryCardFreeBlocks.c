#include "common.h"
#include "game/memcard.h"

extern s32 D_8009B748[];

s32 GameCalculateMemoryCardFreeBlocks(s32 arg0) {
    u8 scratch[8];
    s32 i;
    s32 sum;
    u8 *ptr;
    s32 value;
    register s32 biased asm("$2");

    asm volatile("" : "=m"(scratch));

    i = 0;
    sum = 0;

    if (arg0 > 0) {
        ptr = (u8 *)D_8009B748;
        do {
            value = *(s32 *)(ptr + 0x18);
            asm volatile("" : "=r"(value) : "0"(value));
            i++;
            sum += value;
            ptr += 0x28;
        } while (i < arg0);
    }

    biased = sum;
    if (sum < 0) {
        biased = sum + 0x1FFF;
    }

    return 0xF - (biased >> 13);
}

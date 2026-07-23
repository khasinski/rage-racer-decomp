#include "common.h"
#include "game/memcard.h"

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

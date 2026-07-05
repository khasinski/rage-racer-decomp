#include "common.h"

extern s32 D_801E40A8;
extern volatile u8 D_801E7734[];
extern u8 D_801E7733[];
extern s32 D_8009E6CC;

s32 func_800632B0(void);

void func_8001B488(void) {
    s32 i;
    s32 count;
    s32 j;
    s32 remaining;

    for (i = 0; i < D_801E40A8; i++) {
        D_801E7734[i] = 0xFF;
    }

    for (i = 0; i < D_801E40A8; i++) {
        count = 0;
        for (j = 0; j < D_801E40A8; j++) {
            if (D_801E7734[j] == 0xFF) {
                count++;
            }
        }

        remaining = ((func_800632B0() & 0xFFF) % count) + 1;
        j = 0;
        while (remaining != 0) {
            if (D_801E7734[j] == 0xFF) {
                remaining--;
            }
            j++;
        }
        D_801E7733[j] = i;
    }

    D_8009E6CC = 0;
}

#include "common.h"
#include "game/car.h"

extern GameCarEntry *D_8019C7C8;

s32 func_80050F68(void) {
    s32 count = 0;
    s32 i = 0;
    GameCarEntry *entries = D_8019C7C8;
    s32 flag;

    do {
        flag = entries[i].enabled;
        if (flag != 0) {
            count++;
        }
        i++;
    } while (i < 0xD);

    return count;
}

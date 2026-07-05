#include "common.h"
#include "game/car.h"

extern GameCarEntry *D_8019C7C8;

s32 func_80050F68(void) {
    register s32 count asm("$5") = 0;
    register s32 i asm("$4") = 0;
    register GameCarEntry *ptr asm("$3") = D_8019C7C8;
    s32 flag;

    do {
        flag = ptr->enabled;
        ptr++;
        if (flag != 0) {
            count++;
        }
        i++;
    } while (i < 0xD);

    return count;
}

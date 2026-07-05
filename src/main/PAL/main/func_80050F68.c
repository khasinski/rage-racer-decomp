#include "common.h"

extern u8 *D_8019C7C8;

s32 func_80050F68(void) {
    register s32 count asm("$5") = 0;
    register s32 i asm("$4") = 0;
    register u8 *ptr asm("$3") = D_8019C7C8;
    s32 flag;

    do {
        flag = ptr[5];
        ptr += 8;
        if (flag != 0) {
            count++;
        }
        i++;
    } while (i < 0xD);

    return count;
}

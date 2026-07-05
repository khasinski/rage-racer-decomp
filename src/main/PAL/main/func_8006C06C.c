#include "common.h"
#include "psyq/cd.h"

extern volatile u8 *D_80099300;
extern volatile u8 *D_8009930C;
extern volatile u32 *D_80099310;
extern volatile u32 *D_80099334;
extern volatile u32 *D_80099338;
extern volatile u32 *D_8009933C;
extern volatile u32 *D_80099340;
extern volatile u32 *D_80099344;

s32 CD_getsector2(s32 arg0, u32 arg1) {
    volatile u8 *status;
    volatile u32 *temp_v1;
    volatile u32 *temp_a0;

    *D_80099300 = 0;
    *D_8009930C = 0x80;
    *D_80099334 = 0x20943;
    *D_80099310 = 0x1323;

    temp_v1 = D_80099338;
    *temp_v1 |= 0x8000;

    *D_8009933C = arg0;
    *D_80099340 = arg1 | 0x10000;

    status = D_80099300;
    while ((*status & 0x40) == 0) {
    }

    *D_80099344 = 0x11000000;

    temp_a0 = D_80099344;
    if ((*temp_a0 & 0x1000000) != 0) {
        temp_v1 = temp_a0;
        do {
        } while ((*temp_v1 & 0x1000000) != 0);
    }

    *D_80099310 = 0x1325;
    return 0;
}

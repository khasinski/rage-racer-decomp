#include "common.h"

extern volatile u32 *D_800942B8;
extern volatile u32 *D_800942BC;

s32 func_800680A4(u32 arg0) {
    volatile u32 *gp0;
    u32 status;

    *D_800942BC = 0x10000007;
    gp0 = D_800942B8;
    status = *gp0 & 0xFFFFFF;

    if (status != 2) {
        *gp0 = (*D_800942BC & 0x3FFF) | 0xE1001000;
        *D_800942B8;

        if ((*D_800942BC & 0x1000) == 0) {
            return 0;
        }

        if ((arg0 & 8) == 0) {
            return 1;
        }

        *D_800942BC = 0x20000504;
        return 2;
    }

    if ((arg0 & 8) == 0) {
        return 3;
    }

    *D_800942BC = 0x9000001;
    return 4;
}

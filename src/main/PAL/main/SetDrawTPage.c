#include "common.h"

s32 func_800657E4(void);

void func_800650E4(u8 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    register s32 encoded asm("$2");
    s32 mode;

    arg0[3] = 1;
    mode = func_800657E4();
    if (mode == 1) {
        goto high_mode;
    }

    mode = func_800657E4();
    if (mode != 2) {
        goto low_mode;
    }

high_mode:
    {
        register s32 value asm("$3") = 0xE1000000;

        if (arg2 != 0) {
            value = 0xE1000800;
        }
        encoded = arg3 & 0x27FF;
        if (arg1 != 0) {
            encoded |= 0x1000;
        }
        encoded = value | encoded;
        goto done;
    }

low_mode:
    {
        register s32 value asm("$7") = 0xE1000000;

        if (arg2 != 0) {
            value = 0xE1000200;
        }
        encoded = arg3 & 0x9FF;
        if (arg1 != 0) {
            encoded |= 0x400;
        }
        encoded = value | encoded;
    }

done:
    *(u32 *)&arg0[4] = encoded;
}

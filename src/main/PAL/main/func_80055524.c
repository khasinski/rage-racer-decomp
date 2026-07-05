#include "common.h"

extern s32 D_8009B33C;
extern s32 D_8019C7B8;
extern s32 D_8019C7C8;
extern s32 *D_801E4FAC;

s32 func_8001785C(s32 arg0);

void func_80055524(void) {
    register s32 index asm("s0");
    s32 value;
    u8 *ptr;
    register u8 *enabledPtr asm("v1");
    register s32 byte asm("v0");
    register s32 enabledBase asm("v0");

    D_8019C7B8 = -1;

    if (D_8009B33C != 0) {
        index = 12;
        enabledBase = D_8019C7C8;
        enabledPtr = (u8 *)(enabledBase + 0x60);
loop_enabled:
        byte = enabledPtr[5];
        enabledPtr -= 8;
        if (byte == 0) {
            D_8019C7B8 = index;
        }
        index--;
        if (index < 0) {
            goto done;
        }
        goto loop_enabled;
    }

    index = 12;
loop_disabled:
    {
        value = func_8001785C(index);
        {
            register s32 offset asm("a0") = index * 8;
            ptr = (u8 *)(offset + D_8019C7C8);
        }
        if (ptr[5] == 0) {
            if (D_801E4FAC[3] < 4) {
                if ((D_801E4FAC[3] + 1) < value) {
                    index--;
                    goto loop_disabled_check;
                }
            } else if (D_801E4FAC[3] < value) {
                index--;
                goto loop_disabled_check;
            }
            D_8019C7B8 = index;
        }
        index--;
    }
loop_disabled_check:
    if (index >= 0) {
        goto loop_disabled;
    }

done:
}

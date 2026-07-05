#include "common.h"

extern volatile s32 D_8007D790;
extern volatile s32 D_8007D794;
extern volatile s32 D_8007D79C;
extern volatile s32 D_8007D7A0;
extern volatile s32 D_8007D7AC;
extern volatile s32 D_8007D7B4;
extern volatile s32 D_8007D7B8;

s32 func_8006A574(s32 arg0);
s32 func_8006A58C(s32 arg0);
s32 func_8006DD30(s32 arg0);
s32 func_8006A3E8(void);
s32 func_8006A808(s32 arg0, void *arg1, s32 arg2);
s32 func_8002745C(s32 arg0);

s32 func_80027688(s32 arg0, s32 arg1, s32 arg2) {
    register s32 savedArg0 asm("$7");
    register s32 mode asm("$3");
    register s32 value asm("$2");

    value = (s32)&D_8007D79C;
    *(volatile s32 *)value = arg2;
    value = *(volatile s32 *)value;
    mode = value & 0x30;
    savedArg0 = arg0;

    switch (mode) {
    case 0:
        value = 0x200;
        D_8007D7A0 = value;
        break;
    case 0x20:
        value = 0x249;
        D_8007D7A0 = value;
        break;
    default:
        value = 0x246;
        mode = (s32)&D_8007D7A0;
        *(volatile s32 *)mode = value;
        break;
    }

    mode = (s32)&D_8007D79C;
    value = *(volatile s32 *)mode;
    value |= 0x20;
    *(volatile s32 *)mode = value;
    D_8007D794 = arg1;
    D_8007D790 = savedArg0;
    D_8007D7B4 = func_8006A574(0);
    D_8007D7B8 = func_8006A58C(0);
    D_8007D7AC = func_8006DD30(-1);

    if ((func_8006A3E8() & 0xE0) != 0) {
        func_8006A808(9, 0, 0);
    }

    return func_8002745C(0) > 0;
}

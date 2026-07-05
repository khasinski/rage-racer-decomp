#include "common.h"

extern s32 D_8019CB14;
extern s32 D_8009F0A0;
extern u8 *D_8019C900;

void func_8002390C(void);
s32 func_80032F34(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_800172D4(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

void func_800256B8(void) {
    register s32 *scratch asm("$20");
    register void *base asm("$19");
    s32 pkt;
    s32 target;
    s32 value;
    s32 w120;
    s32 two;
    s32 white;
    s32 h1c0;
    register u8 *rawBase asm("$2");

    if (D_8019CB14 != 9) {
        func_8002390C();
    }

    target = 0xF0;
    if (D_8019CB14 == 6) {
        target = 0x1E0;
    }

    value = D_8009F0A0;
    if (value < target) {
        D_8009F0A0 = value + 4;
    } else if (target < value) {
        D_8009F0A0 = value - 4;
    }

    scratch = (s32 *)0x1F800000;
    rawBase = D_8019C900;
    base = rawBase + 0xBC8;
    pkt = *scratch;

    if (D_8019CB14 == 6) {
        w120 = 0x120;
        two = 2;
        white = 0xFF;
        pkt = func_80032F34(base, pkt, 0x10, 0x20, w120, two, white, white, white);
        pkt = func_80032F34(base, pkt, 0x10, 0x1C0, w120, two, white, white, white);
        h1c0 = 0x1C0;
        pkt = func_800172D4(base, pkt, 0x10, 0x20, 0x10, h1c0, white, white, white);
        pkt = func_800172D4(base, pkt, 0x130, 0x20, 0x130, h1c0, white, white, white);
    }

    *scratch = func_80032F34(base, pkt, 0, 0, 0x140, D_8009F0A0, 0x85, 0x15, 0xE);
}

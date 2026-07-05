#include "common.h"

extern volatile s32 *D_80099420;
extern volatile s32 *D_80099424;
extern volatile s32 D_80099428;
extern s32 D_8009942C;
extern s32 D_8009A4EC;

void waitVSync(s32 target, s32 timeoutFrames) asm("func_8006DE78");
s32 VSync(s32 mode) asm("func_8006DD30");

s32 VSync(s32 arg0) {
    s32 oldTimer;
    s32 delta;
    register s32 waitTarget asm("$2");
    s32 waitCount;
    volatile s32 *timer;

    oldTimer = *D_80099420;
    delta = (u16)(*D_80099424 - D_80099428);

    if (arg0 < 0) {
        return D_8009A4EC;
    }

    if (arg0 == 1) {
        return delta;
    }

    if (arg0 > 0) {
        waitTarget = D_8009942C - 1;
        waitTarget += arg0;
    } else {
        waitTarget = D_8009942C;
    }

    if (arg0 > 0) {
        waitCount = arg0 - 1;
    } else {
        waitCount = 0;
    }
    waitVSync(waitTarget, waitCount);

    {
        register volatile s32 *timer2 asm("$2");
        register s32 waitBase asm("$4");

        timer2 = D_80099420;
        oldTimer = *timer2;
        asm volatile("" : "=r"(oldTimer) : "0"(oldTimer));
        waitBase = D_8009A4EC;
        waitVSync(waitBase + 1, 1);
    }

    if (oldTimer & 0x80000) {
        timer = D_80099420;
        if (!((oldTimer ^ *timer) < 0)) {
            do {
            } while (((oldTimer ^ *timer) & 0x80000000) == 0);
        }
    }

    D_8009942C = D_8009A4EC;
    D_80099428 = *D_80099424;
    return delta;
}

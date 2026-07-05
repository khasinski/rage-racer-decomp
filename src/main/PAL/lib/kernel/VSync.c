#include "psyq/kernel.h"

typedef void (*Callback2)(s32, s32);

typedef struct {
    u8 pad0[0xC];
    void (*callback)(void);
} CallbackTable;

extern volatile s32 *D_80099420;
extern volatile s32 *D_80099424;
extern volatile s32 D_80099428;
extern s32 D_8009942C;
extern u16 D_80099432;
extern volatile u16 *D_8009A4C0;
extern KernelCallback *D_8009A4B8;
extern volatile s32 D_8009A4EC;
extern char D_80013B2C[];

void func_80063C38(char *arg0);

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

void waitVSync(s32 arg0, s32 arg1) {
    volatile s32 timeout;

    timeout = arg1 << 15;
    if (D_8009A4EC < arg0) {
        do {
            if (--timeout == -1) {
                func_80063C38(D_80013B2C);
                ChangeClearRCnt(0);
                ChangeClearInterruptMask(3, 0);
                break;
            }
        } while (D_8009A4EC < arg0);
    }
}

u32 ChangeClearRCntStub[4] asm("func_8006DF14") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x2409005B,
    0,
};

u32 ChangeClearInterruptMaskStub[4] asm("func_8006DF24") __attribute__((section(".text"))) = {
    0x240A00C0,
    0x01400008,
    0x2409000A,
    0,
};

void KernelCallbackSlot3(void) {
    ((CallbackTable *)D_8009A4B8)->callback();
}

void KernelCallbackSlot2(void) {
    D_8009A4B8[2]();
}

void ResetCallback(s32 arg0, s32 arg1) {
    D_8009A4B8[1]();
}

void VSyncCallback(s32 arg0) {
    ((Callback2)D_8009A4B8[5])(0, arg0);
}

void KernelCallbackSlot5(void) {
    D_8009A4B8[5]();
}

void KernelCallbackSlot4(void) {
    D_8009A4B8[4]();
}

void KernelCallbackSlot6(void) {
    D_8009A4B8[6]();
}

s32 GetKernelStatus(void) {
    return D_80099432;
}

s32 GetIntrMask(void) {
    return *D_8009A4C0;
}

s32 SetIntrMask(s32 arg0) {
    u16 value;
    volatile u16 *ptr;

    ptr = D_8009A4C0;
    value = *ptr;
    *ptr = arg0;
    return value;
}

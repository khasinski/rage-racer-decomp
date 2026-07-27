#include "psyq/kernel.h"

typedef void (*Callback2)(long, long);

typedef struct {
    u_char pad0[0xC];
    void (*callback)(void);
} CallbackTable;

extern volatile long *D_80099420;
extern volatile long *D_80099424;
extern volatile long D_80099428;
extern long D_8009942C;
extern u_short D_80099432;
extern volatile u_short *D_8009A4C0;
extern KernelCallback *D_8009A4B8;
extern volatile long D_8009A4EC;
extern char D_80013B2C[];

void func_80063C38(char *arg0);

long VSync(long arg0) {
    long oldTimer;
    long delta;
    register long waitTarget asm("$2");
    long waitCount;
    volatile long *timer;

    oldTimer = *D_80099420;
    delta = (u_short)(*D_80099424 - D_80099428);

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
        register volatile long *timer2 asm("$2");
        register long waitBase asm("$4");

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

void waitVSync(long arg0, long arg1) {
    volatile long timeout;

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

u_long ChangeClearRCntStub[4] asm("func_8006DF14") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x2409005B,
    0,
};

u_long ChangeClearInterruptMaskStub[4] asm("func_8006DF24") __attribute__((section(".text"))) = {
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

void DMACallback(long arg0, long arg1) {
    D_8009A4B8[1]();
}

void VSyncCallback(long arg0) {
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

long GetKernelStatus(void) {
    return D_80099432;
}

long GetIntrMask(void) {
    return *D_8009A4C0;
}

long SetIntrMask(long arg0) {
    u_short value;
    volatile u_short *ptr;

    ptr = D_8009A4C0;
    value = *ptr;
    *ptr = arg0;
    return value;
}

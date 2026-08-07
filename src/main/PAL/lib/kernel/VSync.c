#include "psyq/kernel.h"

typedef void (*Callback2)(long, long);

typedef struct CallbackTable {
    u_char pad0[0xC];
    void (*callback)(void);
} CallbackTable;

extern KernelCallback *g_IntrRpNode;

void LibcPutString(char *arg0);

long VSync(long arg0) {
    long oldTimer;
    long delta;
    long waitTarget;
    long waitCount;
    long one;
    volatile long *timer;

    oldTimer = *g_VSyncGpuStat;
    delta = (u_short)(*g_Timer1CountReg - g_VSyncTimerBase);

    if (arg0 < 0) {
        return g_VSyncCount;
    }

    if (arg0 == 1) {
        return delta;
    }

    one = 1;
    waitTarget = arg0 > 0 ? g_VSyncCountBase - one + arg0 : g_VSyncCountBase;
    waitCount = arg0 > 0 ? arg0 - one : 0;
    waitVSync(waitTarget, waitCount);

    {
        volatile long *timer2;
        long waitBase;

        timer2 = g_VSyncGpuStat;
        oldTimer = *timer2;
        waitBase = g_VSyncCount;
        waitVSync(waitBase + 1, 1);
    }

    if (oldTimer & 0x80000) {
        timer = g_VSyncGpuStat;
        if (!((oldTimer ^ *timer) < 0)) {
            do {
            } while (((oldTimer ^ *timer) & 0x80000000) == 0);
        }
    }

    g_VSyncCountBase = g_VSyncCount;
    g_VSyncTimerBase = *g_Timer1CountReg;
    return delta;
}

void waitVSync(long arg0, long arg1) {
    volatile long timeout;

    timeout = arg1 << 15;
    if (g_VSyncCount < arg0) {
        do {
            if (--timeout == -1) {
                LibcPutString(D_80013B2C);
                ChangeClearRCnt(0);
                ChangeClearInterruptMask(3, 0);
                break;
            }
        } while (g_VSyncCount < arg0);
    }
}

u_long ChangeClearRCntStub[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x2409005B,
    0,
};

u_long ChangeClearInterruptMaskStub[4] __attribute__((section(".text"))) = {
    0x240A00C0,
    0x01400008,
    0x2409000A,
    0,
};

void KernelCallbackSlot3(void) {
    ((CallbackTable *)g_IntrRpNode)->callback();
}

void KernelCallbackSlot2(void) {
    g_IntrRpNode[2]();
}

void DMACallback(long arg0, long arg1) {
    g_IntrRpNode[1]();
}

void VSyncCallback(long arg0) {
    ((Callback2)g_IntrRpNode[5])(0, arg0);
}

void KernelCallbackSlot5(void) {
    g_IntrRpNode[5]();
}

void KernelCallbackSlot4(void) {
    g_IntrRpNode[4]();
}

void KernelCallbackSlot6(void) {
    g_IntrRpNode[6]();
}

long GetKernelStatus(void) {
    return g_IntrInDispatch;
}

long GetIntrMask(void) {
    return *g_IrqMask;
}

long SetIntrMask(long arg0) {
    u_short value;
    volatile u_short *ptr;

    ptr = g_IrqMask;
    value = *ptr;
    *ptr = arg0;
    return value;
}

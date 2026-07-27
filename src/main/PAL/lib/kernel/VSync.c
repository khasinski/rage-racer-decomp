#include "psyq/kernel.h"

typedef void (*Callback2)(long, long);

typedef struct {
    u_char pad0[0xC];
    void (*callback)(void);
} CallbackTable;

/* VSync bookkeeping. g_VSyncGpuStat is GP1/GPUSTAT 0x1F801814 (the field bit
 * is what the busy-wait watches) and g_Timer1CountReg is T1_COUNT 0x1F801110;
 * the two *Base words are the counter values sampled at the last VSync(). */
extern volatile long *g_VSyncGpuStat asm("D_80099420");
extern volatile long *g_Timer1CountReg asm("D_80099424");
extern volatile long g_VSyncTimerBase asm("D_80099428");
extern long g_VSyncCountBase asm("D_8009942C");
extern u_short g_IntrInDispatch asm("D_80099432");
extern volatile u_short *g_IrqMask asm("D_8009A4C0");
extern KernelCallback *g_IntrRpNode asm("D_8009A4B8");
extern volatile long g_VSyncCount asm("D_8009A4EC");
extern char D_80013B2C[];

void func_80063C38(char *arg0);

long VSync(long arg0) {
    long oldTimer;
    long delta;
    /* This pin is load-bearing: removing it changes .text. */
    register long waitTarget asm("$2");
    long waitCount;
    volatile long *timer;

    oldTimer = *g_VSyncGpuStat;
    delta = (u_short)(*g_Timer1CountReg - g_VSyncTimerBase);

    if (arg0 < 0) {
        return g_VSyncCount;
    }

    if (arg0 == 1) {
        return delta;
    }

    if (arg0 > 0) {
        waitTarget = g_VSyncCountBase - 1;
        waitTarget += arg0;
    } else {
        waitTarget = g_VSyncCountBase;
    }

    if (arg0 > 0) {
        waitCount = arg0 - 1;
    } else {
        waitCount = 0;
    }
    waitVSync(waitTarget, waitCount);

    {
        volatile long *timer2;
        long waitBase;

        timer2 = g_VSyncGpuStat;
        oldTimer = *timer2;
        asm volatile("" : "=r"(oldTimer) : "0"(oldTimer));
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
                func_80063C38(D_80013B2C);
                ChangeClearRCnt(0);
                ChangeClearInterruptMask(3, 0);
                break;
            }
        } while (g_VSyncCount < arg0);
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

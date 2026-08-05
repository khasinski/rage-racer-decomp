#include "psyq/kernel.h"

void *StartKernelInterrupts(void) {
    u_short *state;

    state = g_IntrState;
    if (state[0] != 0) {
        return 0;
    }

    HookEntryInt(&state[0x1C]);
    {
        volatile u_short *mask = g_IrqMask;
        u_short pending = g_IntrSavedIrqMask;

        state[0] = 1;
        *mask = pending;
    }
    *g_KernelDpcr = g_IntrSavedDpcr;
    ExitCriticalSection();

    return state;
}

extern void *g_VSyncCallbacks[];
extern u_long *g_Timer1ModeReg;
extern volatile u_long *g_DmaIrqControl;
extern u_long g_DmaCallbacks[];
extern u_long *g_DmaChannelRegs;
extern long D_8009A51C;
extern u_char D_80013BA8[];
extern u_char D_80013BC4[];

void RegisterKernelCallback(long slot, void *callback) asm("KernelCallbackSlot2");
void *setIntrVSyncAddress(void) asm("setIntrVSync");
void *setIntrDMAAddress(void) asm("setIntrDMA");

void clearKernelInterruptState(u_long *dst, long count) {
    volatile long unused;
    long i = count - 1;

    if (count != 0) {
        do {
            *dst = 0;
            i--;
            dst++;
        } while (i != -1);
    }
}

u_long SysEnqIntRPStub[31] __attribute__((section(".text"))) = {
    0x240A00A0,
    0x01400008,
    0x24090072,
    0,
    0x240A00B0,
    0x01400008,
    0x24090017,
    0,
    0x240A00B0,
    0x01400008,
    0x24090018,
    0,
    0x240A00B0,
    0x01400008,
    0x24090019,
    0,
    0xAC9F0000,
    0xAC9C002C,
    0xAC9D0004,
    0xAC9E0008,
    0xAC90000C,
    0xAC910010,
    0xAC920014,
    0xAC930018,
    0xAC94001C,
    0xAC950020,
    0xAC960024,
    0xAC970028,
    0x00001021,
    0x03E00008,
    0,
};

u_long RestoreKernelRegistersStub[17] asm("RestoreKernelRegisters") __attribute__((section(".text"))) = {
    0x8C9F0000,
    0x8C9C002C,
    0x8C9D0004,
    0x8C9E0008,
    0x8C90000C,
    0x8C910010,
    0x8C920014,
    0x8C930018,
    0x8C94001C,
    0x8C950020,
    0x8C960024,
    0x8C970028,
    0x00A01021,
    0x03E00008,
    0,
    0,
    0,
};

void *startIntrVSync(void) {
    *g_Timer1ModeReg = 0x107;
    g_VSyncCount = 0;
    clearIntrVSyncCallbacks((u_long *)g_VSyncCallbacks, 8);
    RegisterKernelCallback(0, intrVSyncDispatcher);

    return setIntrVSyncAddress;
}

void intrVSyncDispatcher(void) {
    long i;
    void (**callback)(void);
    void (*func)(void);
    long count;

    count = g_VSyncCount;
    i = 0;
    callback = (void (**)(void))g_VSyncCallbacks;
    g_VSyncCount = count + 1;
    count = g_VSyncCount;
    for (; i < 8; i++) {
        func = *callback++;
        if (func != 0) {
            func();
        }
    }
}

void setIntrVSync(long arg0, void *arg1) {
    void **base;
    void **slot;

    base = g_VSyncCallbacks;
    slot = &base[arg0];
    if (arg1 != *slot) {
        *slot = arg1;
    }
}

void clearIntrVSyncCallbacks(u_long *dst, long count) {
    volatile long unused;
    long i = count - 1;

    if (count != 0) {
        do {
            *dst = 0;
            i--;
            dst++;
        } while (i != -1);
    }
}

void *startIntrDMA(void) {
    clearIntrDMACallbacks(g_DmaCallbacks, 8);
    *g_DmaIrqControl = 0;
    RegisterKernelCallback(3, intrDMADispatcher);

    return setIntrDMAAddress;
}

void intrDMADispatcher(void) {
    u_long pending;
    u_long pendingTemp;
    long i;
    void (**handler)(void);
    u_long lowMask;
    u_long one;
    void (**handlerBase)(void);
    u_char *fmt;

    pendingTemp = *g_DmaIrqControl;
    pending = (pendingTemp >> 0x18) & 0x7F;
    if (pending != 0) {
        one = 1;
        lowMask = 0xFFFFFF;
        handlerBase = (void (**)(void))g_DmaCallbacks;
        do {
            i = 0;
            if (pending != 0) {
                handler = handlerBase;
                while ((pending != 0) && (i < 7)) {
                    if (pending & 1) {
                        volatile u_long *bits;
                        u_long value;
                        long shift;

                        bits = g_DmaIrqControl;
                        shift = i + 0x18;
                        value = one << shift;
                        value |= lowMask;
                        value &= *bits;
                        *bits = value;
                        if (*handler != 0) {
                            (*handler)();
                        }
                    }
                    handler++;
                    pending >>= 1;
                    i++;
                }
            }

            pendingTemp = *g_DmaIrqControl;
            pending = (pendingTemp >> 0x18) & 0x7F;
        } while (pending != 0);
    }

    if (((*g_DmaIrqControl & 0xFF000000) == 0x80000000) || ((*g_DmaIrqControl & 0x8000) != 0)) {
        fmt = D_80013BA8;
        DebugPrintf(fmt, *g_DmaIrqControl);
        for (i = 0; i < 7; i++) {
            DebugPrintf(D_80013BC4, i, g_DmaChannelRegs[i * 4]);
        }
    }
}

u_long setIntrDMA(long arg0, u_long arg1) {
    long index;
    u_long callback;
    u_long *base;
    long offset;
    u_long *slot;
    u_long oldCallback;

    index = arg0;
    base = g_DmaCallbacks;
    offset = index << 2;
    slot = (u_long *)((long)base + offset);
    oldCallback = *slot;
    callback = arg1;

    if (callback != oldCallback) {
        if (callback != 0) {
            volatile u_long *bits = g_DmaIrqControl;
            u_long value;
            register long shift asm("$3");
            register u_long mask asm("$2") = 0xFFFFFF;

            *slot = callback;
            value = *bits;
            shift = index + 0x10;
            value &= mask;
            mask = 1;
            mask <<= shift;
            shift = 0x800000;
            mask |= shift;
            value |= mask;
            *bits = value;
        } else {
            register volatile u_long *bits asm("$5") = g_DmaIrqControl;
            u_long value;
            long shift;
            register u_long mask asm("$2") = 0xFFFFFF;
            register u_long zero asm("$0");

            *slot = zero;
            value = *bits;
            shift = index + 0x10;
            value &= mask;
            mask = 0x800000;
            value |= mask;
            mask = 1;
            mask <<= shift;
            mask = ~mask;
            value &= mask;
            *bits = value;
        }
    }

    return oldCallback;
}

void clearIntrDMACallbacks(u_long *dst, long count) {
    volatile long unused;
    long i = count - 1;

    if (count != 0) {
        do {
            *dst = 0;
            i--;
            dst++;
        } while (i != -1);
    }
}

long SetDMAInterruptState(long arg0) {
    long value;

    value = D_8009A51C;
    D_8009A51C = arg0;
    return value;
}

long GetDMAInterruptState(void) {
    return D_8009A51C;
}

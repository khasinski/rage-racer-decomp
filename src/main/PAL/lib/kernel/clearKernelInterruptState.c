#include "psyq/kernel.h"

extern void *g_VSyncCallbacks[] asm("D_8009A4CC");
extern u_long *g_Timer1ModeReg asm("D_8009A4F0");
extern volatile long g_VSyncCount asm("D_8009A4EC");
extern volatile u_long *g_DmaIrqControl asm("D_8009A4F4");
extern u_long g_DmaCallbacks[] asm("D_8009A4F8");
extern u_long *g_DmaChannelRegs asm("D_8009A518");
extern long D_8009A51C;
extern u_char D_80013BA8[];
extern u_char D_80013BC4[];

void RegisterKernelCallback(long slot, void *callback) asm("func_8006DF64");
void *setIntrVSyncAddress(void) asm("func_8006E7D4");
void *setIntrDMAAddress(void) asm("func_8006EA00");

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

u_long SysEnqIntRPStub[31] asm("func_8006E644") __attribute__((section(".text"))) = {
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

u_long RestoreKernelRegistersStub[17] asm("func_8006E6C0") __attribute__((section(".text"))) = {
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
    register long i asm("$17");
    register void (**callback)(void) asm("$16");
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
    register void **base asm("$2");
    register void **slot asm("$4");

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
    register void (**handlerBase)(void) asm("$21");
    register u_char *fmt asm("$4");

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
                        register volatile u_long *bits asm("$4");
                        register u_long value asm("$2");
                        register long shift asm("$2");

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
        GameDebugPrintf(fmt, *g_DmaIrqControl);
        for (i = 0; i < 7; i++) {
            GameDebugPrintf(D_80013BC4, i, g_DmaChannelRegs[i * 4]);
        }
    }
}

u_long setIntrDMA(long arg0, u_long arg1) {
    register long index asm("$6");
    register u_long callback asm("$4");
    register u_long *base asm("$3");
    register long offset asm("$2");
    register u_long *slot asm("$3");
    register u_long oldCallback asm("$7");

    index = arg0;
    asm("");
    base = g_DmaCallbacks;
    offset = index << 2;
    slot = (u_long *)((long)base + offset);
    oldCallback = *slot;
    callback = arg1;

    if (callback != oldCallback) {
        if (callback != 0) {
            register volatile u_long *bits asm("$5") = g_DmaIrqControl;
            register u_long value asm("$4");
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
            register u_long value asm("$3");
            register long shift asm("$4");
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

#include "psyq/kernel.h"

extern void *D_8009A4CC[];
extern u32 *D_8009A4F0;
extern volatile s32 D_8009A4EC;
extern volatile u32 *D_8009A4F4;
extern u32 D_8009A4F8[];
extern u32 *D_8009A518;
extern s32 D_8009A51C;
extern u8 D_80013BA8[];
extern u8 D_80013BC4[];

void RegisterKernelCallback(s32 slot, void *callback) asm("func_8006DF64");
void *setIntrVSyncAddress(void) asm("func_8006E7D4");
void *setIntrDMAAddress(void) asm("func_8006EA00");
void func_8001674C(u8 *arg0, ...);

void clearKernelInterruptState(u32 *dst, s32 count) {
    volatile s32 unused;
    s32 i = count - 1;

    if (count != 0) {
        do {
            *dst = 0;
            i--;
            dst++;
        } while (i != -1);
    }
}

u32 SysEnqIntRPStub[31] asm("func_8006E644") __attribute__((section(".text"))) = {
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

u32 RestoreKernelRegistersStub[17] asm("func_8006E6C0") __attribute__((section(".text"))) = {
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
    *D_8009A4F0 = 0x107;
    D_8009A4EC = 0;
    clearIntrVSyncCallbacks((u32 *)D_8009A4CC, 8);
    RegisterKernelCallback(0, intrVSyncDispatcher);

    return setIntrVSyncAddress;
}

void intrVSyncDispatcher(void) {
    register s32 i asm("$17");
    register void (**callback)(void) asm("$16");
    void (*func)(void);
    s32 count;

    count = D_8009A4EC;
    i = 0;
    callback = (void (**)(void))D_8009A4CC;
    D_8009A4EC = count + 1;
    count = D_8009A4EC;
    for (; i < 8; i++) {
        func = *callback++;
        if (func != 0) {
            func();
        }
    }
}

void setIntrVSync(s32 arg0, void *arg1) {
    register void **base asm("$2");
    register void **slot asm("$4");

    base = D_8009A4CC;
    slot = &base[arg0];
    if (arg1 != *slot) {
        *slot = arg1;
    }
}

void clearIntrVSyncCallbacks(u32 *dst, s32 count) {
    volatile s32 unused;
    s32 i = count - 1;

    if (count != 0) {
        do {
            *dst = 0;
            i--;
            dst++;
        } while (i != -1);
    }
}

void *startIntrDMA(void) {
    clearIntrDMACallbacks(D_8009A4F8, 8);
    *D_8009A4F4 = 0;
    RegisterKernelCallback(3, intrDMADispatcher);

    return setIntrDMAAddress;
}

void intrDMADispatcher(void) {
    u32 pending;
    u32 pendingTemp;
    s32 i;
    void (**handler)(void);
    u32 lowMask;
    u32 one;
    register void (**handlerBase)(void) asm("$21");
    register u8 *fmt asm("$4");

    pendingTemp = *D_8009A4F4;
    pending = (pendingTemp >> 0x18) & 0x7F;
    if (pending != 0) {
        one = 1;
        lowMask = 0xFFFFFF;
        handlerBase = (void (**)(void))D_8009A4F8;
        do {
            i = 0;
            if (pending != 0) {
                handler = handlerBase;
                while ((pending != 0) && (i < 7)) {
                    if (pending & 1) {
                        register volatile u32 *bits asm("$4");
                        register u32 value asm("$2");
                        register s32 shift asm("$2");

                        bits = D_8009A4F4;
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

            pendingTemp = *D_8009A4F4;
            pending = (pendingTemp >> 0x18) & 0x7F;
        } while (pending != 0);
    }

    if (((*D_8009A4F4 & 0xFF000000) == 0x80000000) || ((*D_8009A4F4 & 0x8000) != 0)) {
        fmt = D_80013BA8;
        func_8001674C(fmt, *D_8009A4F4);
        for (i = 0; i < 7; i++) {
            func_8001674C(D_80013BC4, i, D_8009A518[i * 4]);
        }
    }
}

u32 setIntrDMA(s32 arg0, u32 arg1) {
    register s32 index asm("$6");
    register u32 callback asm("$4");
    register u32 *base asm("$3");
    register s32 offset asm("$2");
    register u32 *slot asm("$3");
    register u32 oldCallback asm("$7");

    index = arg0;
    asm("");
    base = D_8009A4F8;
    offset = index << 2;
    slot = (u32 *)((s32)base + offset);
    oldCallback = *slot;
    callback = arg1;

    if (callback != oldCallback) {
        if (callback != 0) {
            register volatile u32 *bits asm("$5") = D_8009A4F4;
            register u32 value asm("$4");
            register s32 shift asm("$3");
            register u32 mask asm("$2") = 0xFFFFFF;

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
            register volatile u32 *bits asm("$5") = D_8009A4F4;
            register u32 value asm("$3");
            register s32 shift asm("$4");
            register u32 mask asm("$2") = 0xFFFFFF;
            register u32 zero asm("$0");

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

void clearIntrDMACallbacks(u32 *dst, s32 count) {
    volatile s32 unused;
    s32 i = count - 1;

    if (count != 0) {
        do {
            *dst = 0;
            i--;
            dst++;
        } while (i != -1);
    }
}

s32 SetDMAInterruptState(s32 arg0) {
    s32 value;

    value = D_8009A51C;
    D_8009A51C = arg0;
    return value;
}

s32 GetDMAInterruptState(void) {
    return D_8009A51C;
}

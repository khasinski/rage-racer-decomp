#include "psyq/kernel.h"

long SetRCnt(long arg0, long arg1, long arg2) {
    register long index asm("$8") = arg0 & 0xFFFF;
    u_short flags = 0x48;
    long offset;
    volatile u_short *entry;
    long base_v0;
    long ret;
    register long base asm("$4");
    long small;

    if (index >= 3) {
        return 0;
    }

    base_v0 = (long)g_RootCounterRegs;
    offset = index << 4;
    entry = (volatile u_short *)(offset + base_v0);
    small = (u_long)index < 2U;
    entry[2] = 0;
    entry[4] = arg1;

    if (small) {
        if (arg2 & 0x10) {
            flags = 0x49;
        }
        if (!(arg2 & 1)) {
            flags |= 0x100;
        }
    } else if (index == 2) {
        if (!(arg2 & 1)) {
            flags = 0x248;
        }
    }

    offset = index << 4;
    if (arg2 & 0x1000) {
        flags |= 0x10;
    }

    ret = 1;
    asm("" : "=r"(ret) : "0"(ret));
    base = (long)g_RootCounterRegs;
    entry = (volatile u_short *)(offset + base);
    entry[2] = flags;
    return ret;
}

long GetRCnt(long arg0) {
    long index;

    index = arg0 & 0xFFFF;
    if (index >= 3) {
        return 0;
    }
    return g_RootCounterRegs[index * 8];
}

long StartRCnt(long arg0) {
    long index;

    index = arg0 & 0xFFFF;
    g_IrqRegs[1] |= g_RootCounterIrqBits[index];
    return index < 3;
}

long StopRCnt(long arg0) {
    long index;

    index = arg0 & 0xFFFF;
    g_IrqRegs[1] = ~g_RootCounterIrqBits[index] & g_IrqRegs[1];
    return 1;
}

long ResetRCnt(long arg0) {
    long index;

    index = arg0 & 0xFFFF;
    if (index >= 3) {
        return 0;
    }
    g_RootCounterRegs[index * 8] = 0;
    return 1;
}

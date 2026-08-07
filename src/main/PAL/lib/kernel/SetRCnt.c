#include "psyq/kernel.h"

long SetRCnt(long arg0, long arg1, long arg2) {
    long index = arg0 & 0xFFFF;
    long flags = 0x48;

    if (index >= 3) {
        return 0;
    }

    g_RootCounterRegs[index].mode = 0;
    g_RootCounterRegs[index].target = arg1;

    if ((u_long)index < 2U) {
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

    if (arg2 & 0x1000) {
        flags |= 0x10;
    }

    g_RootCounterRegs[index].mode = flags;
    return 1;
}

long GetRCnt(long arg0) {
    long index;

    index = arg0 & 0xFFFF;
    if (index >= 3) {
        return 0;
    }
    return g_RootCounterRegs[index].count;
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
    g_RootCounterRegs[index].count = 0;
    return 1;
}

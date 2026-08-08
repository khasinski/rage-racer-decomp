#include "psyq/spu.h"
#include "psyq/kernel.h"

extern long g_SpuTransferCallback;

u_long Spu_ReadFromSpu(long addr, u_long requested) {
    u_long size = requested;

    if (0x7F000 < size) {
        size = 0x7F000;
    }

    _spu_Fw(addr, size);

    if (g_SpuTransferCallback == 0) {
        g_SpuTransferCompleted = 0;
    }

    return size;
}

long SpuSetTransferStartAddr(long addr) {
    g_SpuTransferStartAddr = _spu_FsetRXXa(-1, addr);
    return addr;
}

long SpuSetTransferMode(long mode) {
    long transferByIo;

    switch (mode) {
    case 0:
        transferByIo = 0;
        break;
    case 1:
        transferByIo = 1;
        break;
    default:
        transferByIo = 0;
        break;
    }

    g_SpuTransferMode = mode;
    g_SpuTransferByIo = transferByIo;
    return transferByIo;
}

long SpuIsTransferCompleted(long wait) {
    long one;
    long saved_arg;
    long ret;

    saved_arg = wait;
    one = 1;

    if (g_SpuTransferMode == one) {
        ret = one;
    } else if (g_SpuTransferCompleted == one) {
        ret = one;
    } else {
        ret = TestEvent(g_SpuTransferEvent);

        if (saved_arg == one) {
            if (ret == 0) {
                do {
                    ret = TestEvent(g_SpuTransferEvent);
                } while (ret == 0);
            }

            ret = 1;
            g_SpuTransferCompleted = ret;
        } else if (ret == one) {
            g_SpuTransferCompleted = ret;
        }
    }

    return ret;
}

void _spu_setTransferCompletionFlag(long completed) {
    if (completed == 1) {
        g_SpuTransferCompleted = 0;
    } else {
        g_SpuTransferCompleted = 1;
    }
}

u_long _spu_isTransferIdle(void) {
    return g_SpuTransferCompleted == 0;
}

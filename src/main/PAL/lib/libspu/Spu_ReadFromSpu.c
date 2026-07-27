#include "psyq/spu.h"
#include "psyq/kernel.h"

extern long g_SpuTransferMode asm("D_8009A714");
extern long g_SpuTransferEvent asm("D_8009A768");
extern u_short g_SpuTransferStartAddr asm("D_8009AB78");
extern long g_SpuTransferByIo asm("D_8009AB94");
extern long g_SpuTransferCompleted asm("D_8009ABAC");
extern long g_SpuTransferCallback asm("D_8009ABB0");

u_long Spu_ReadFromSpu(long arg0, u_long arg1) {
    u_long size = arg1;

    if (0x7F000 < size) {
        size = 0x7F000;
    }

    _spu_Fw(arg0, size);

    if (g_SpuTransferCallback == 0) {
        g_SpuTransferCompleted = 0;
    }

    return size;
}

long SpuSetTransferStartAddr(long arg0) {
    g_SpuTransferStartAddr = _spu_FsetRXXa(-1, arg0);
    return arg0;
}

void SpuSetTransferMode(long arg0) {
    /* This pin is load-bearing: removing it changes .text. */
    register long value asm("$2");

    value = 1;
    if (arg0 == 0) {
        value = 0;
    } else {
        if (arg0 == value) {
            value = 1;
        } else {
            value = 0;
        }
    }

    g_SpuTransferMode = arg0;
    g_SpuTransferByIo = value;
}

long SpuIsTransferCompleted(long arg0) {
    long one;
    long saved_arg;
    long ret;

    saved_arg = arg0;
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

void _spu_setTransferCompletionFlag(long arg0) {
    if (arg0 == 1) {
        g_SpuTransferCompleted = 0;
    } else {
        g_SpuTransferCompleted = 1;
    }
}

u_long _spu_isTransferIdle(void) {
    return g_SpuTransferCompleted == 0;
}

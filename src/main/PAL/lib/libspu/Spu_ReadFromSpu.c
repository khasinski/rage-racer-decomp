#include "psyq/spu.h"
#include "psyq/kernel.h"

extern long D_8009A714;
extern long D_8009A768;
extern u_short D_8009AB78;
extern long D_8009AB94;
extern long D_8009ABAC;
extern long D_8009ABB0;

u_long Spu_ReadFromSpu(long arg0, u_long arg1) {
    u_long size = arg1;

    if (0x7F000 < size) {
        size = 0x7F000;
    }

    _spu_Fw(arg0, size);

    if (D_8009ABB0 == 0) {
        D_8009ABAC = 0;
    }

    return size;
}

long SpuSetTransferStartAddr(long arg0) {
    D_8009AB78 = _spu_FsetRXXa(-1, arg0);
    return arg0;
}

void SpuSetTransferMode(long arg0) {
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

    D_8009A714 = arg0;
    D_8009AB94 = value;
}

long SpuIsTransferCompleted(long arg0) {
    register long one asm("$16");
    register long saved_arg asm("$17");
    register long ret asm("$2");

    saved_arg = arg0;
    one = 1;

    if (D_8009A714 == one) {
        ret = one;
    } else if (D_8009ABAC == one) {
        ret = one;
    } else {
        ret = TestEvent(D_8009A768);

        if (saved_arg == one) {
            if (ret == 0) {
                do {
                    ret = TestEvent(D_8009A768);
                } while (ret == 0);
            }

            ret = 1;
            D_8009ABAC = ret;
        } else if (ret == one) {
            D_8009ABAC = ret;
        }
    }

    return ret;
}

void _spu_setTransferCompletionFlag(long arg0) {
    if (arg0 == 1) {
        D_8009ABAC = 0;
    } else {
        D_8009ABAC = 1;
    }
}

u_long _spu_isTransferIdle(void) {
    return D_8009ABAC == 0;
}

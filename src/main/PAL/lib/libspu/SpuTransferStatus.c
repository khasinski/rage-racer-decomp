#include <sys/types.h>

#include "common.h"

extern volatile u_short *D_8009AB7C;

void _spu_startDmaTransfer(long arg0, long arg1, long arg2) asm("func_80079420");

long SpuTransferStatus(long arg0, long arg1) asm("func_8007B014");

long SpuTransferStatus(long arg0, long arg1) {
    long size;
    u_long status;

    switch (arg1) {
    case 5:
        arg1 = 0;
        size = 0x20;
        break;
    case 6:
        arg1 = 0x100;
        arg0 += 0x800;
        size = 0x20;
        break;
    default:
        arg1 = 0;
        size = 0x40;
        break;
    }

    _spu_startDmaTransfer(arg0, arg1, size);
    status = D_8009AB7C[0xD7] & 0x800;
    return status > 0;
}

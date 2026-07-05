#include "common.h"

extern volatile u16 *D_8009AB7C;

void _spu_startDmaTransfer(s32 arg0, s32 arg1, s32 arg2) asm("func_80079420");

s32 SpuTransferStatus(s32 arg0, s32 arg1) asm("func_8007B014");

s32 SpuTransferStatus(s32 arg0, s32 arg1) {
    s32 size;
    u32 status;

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

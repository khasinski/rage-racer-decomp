#include "psyq/spu.h"

extern u_short g_SpuTransferStartAddr asm("D_8009AB78");
extern volatile u_short *g_SpuRegBase asm("D_8009AB7C");
/* Non-volatile view of the same pointer: _spu_FgetRXXa reads the register file
 * through it and the volatile spelling does not match. Same object, one alias. */
extern u_short *g_SpuRegBaseNv asm("D_8009AB7C");
extern long g_SpuTransferByIo asm("D_8009AB94");
extern long g_SpuMemMode asm("D_8009AB9C");
extern long _spu_mem_mode_unitM asm("D_8009ABA0");
extern u_long g_SpuMemModeUnit asm("D_8009ABA4");
extern u_long _spu_mem_mode_plus asm("D_8009ABA8");

long _spu_Fw(long arg0, long arg1) {
    if (g_SpuTransferByIo == 0) {
        _spu_t(2, g_SpuTransferStartAddr << _spu_mem_mode_unitM);
        _spu_t(1);
        _spu_t(3, arg0, arg1);
    } else {
        _spu_writeByIO(arg0, arg1);
    }

    return arg1;
}

long _spu_Fr(long arg0, long arg1) {
    _spu_t(2, g_SpuTransferStartAddr << _spu_mem_mode_unitM);
    _spu_t(0);
    _spu_t(3, arg0, arg1);

    return arg1;
}

void _spu_FsetRXX(long arg0, u_long arg1, long arg2) {
    if (arg2 == 0) {
        g_SpuRegBase[arg0] = arg1;
    } else {
        g_SpuRegBase[arg0] = arg1 >> _spu_mem_mode_unitM;
    }
}

long _spu_FsetRXXa(long arg0, u_long arg1) {
    long index = arg0;
    u_long shifted;
    long shift;
    u_long divisor;

    if (g_SpuMemMode != 0) {
        divisor = g_SpuMemModeUnit;
        if ((arg1 % divisor) != 0) {
            arg1 += divisor;
            arg1 &= ~_spu_mem_mode_plus;
        }
    }

    shift = _spu_mem_mode_unitM;
    shifted = arg1 >> shift;

    if (!(index == -2)) {
    if (index != -1) {
    } else {
    return shifted & 0xFFFF;
    }
    } else {
    return arg1;
    }
    {
        long base = (long)g_SpuRegBase;
        long offset = index << 1;
        *(volatile u_short *)(offset + base) = shifted;
    }
    return arg1;
}

long _spu_FgetRXXa(long arg0, long arg1) {
    register long value asm("a0");
    long ret;

    value = g_SpuRegBaseNv[arg0];
    if (arg1 != -1) {
        ret = value << _spu_mem_mode_unitM;
    } else {
        ret = value;
    }

    return ret;
}

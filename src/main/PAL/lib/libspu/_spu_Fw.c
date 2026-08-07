#include "psyq/spu.h"

extern volatile u_short *g_SpuRegBase;
extern u_long g_SpuMemModeUnit;
extern u_long _spu_mem_mode_plus;

long _spu_Fw(long addr, long size) {
    if (g_SpuTransferByIo == 0) {
        _spu_t(2, g_SpuTransferStartAddr << _spu_mem_mode_unitM);
        _spu_t(1);
        _spu_t(3, addr, size);
    } else {
        _spu_writeByIO(addr, size);
    }

    return size;
}

long _spu_Fr(long addr, long size) {
    _spu_t(2, g_SpuTransferStartAddr << _spu_mem_mode_unitM);
    _spu_t(0);
    _spu_t(3, addr, size);

    return size;
}

void _spu_FsetRXX(long reg, u_long value, long use_mem_shift) {
    if (use_mem_shift == 0) {
        g_SpuRegBase[reg] = value;
    } else {
        g_SpuRegBase[reg] = value >> _spu_mem_mode_unitM;
    }
}

long _spu_FsetRXXa(long reg, u_long value) {
    long index = reg;
    u_long shifted;
    long shift;
    u_long divisor;

    if (g_SpuMemMode != 0) {
        divisor = g_SpuMemModeUnit;
        if ((value % divisor) != 0) {
            value += divisor;
            value &= ~_spu_mem_mode_plus;
        }
    }

    shift = _spu_mem_mode_unitM;
    shifted = value >> shift;

    if (!(index == -2)) {
    if (index != -1) {
    } else {
    return shifted & 0xFFFF;
    }
    } else {
    return value;
    }
    {
        long base = (long)g_SpuRegBase;
        long offset = index << 1;
        *(volatile u_short *)(offset + base) = shifted;
    }
    return value;
}

long _spu_FgetRXXa(long reg, long raw) {
    register long value asm("a0");
    long ret;

    value = ((u_short *)g_SpuRegBase)[reg];
    if (raw != -1) {
        ret = value << _spu_mem_mode_unitM;
    } else {
        ret = value;
    }

    return ret;
}

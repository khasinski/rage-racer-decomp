#include "psyq/spu.h"

extern u_short D_8009AB78;
extern volatile u_short *D_8009AB7C;
extern u_short *D_8009AB7C_nonvolatile asm("D_8009AB7C");
extern long D_8009AB94;
extern long D_8009AB9C;
extern long _spu_mem_mode_unitM asm("D_8009ABA0");
extern u_long D_8009ABA4;
extern u_long _spu_mem_mode_plus asm("D_8009ABA8");

long _spu_Fw(long arg0, long arg1) {
    if (D_8009AB94 == 0) {
        _spu_t(2, D_8009AB78 << _spu_mem_mode_unitM);
        _spu_t(1);
        _spu_t(3, arg0, arg1);
    } else {
        _spu_writeByIO(arg0, arg1);
    }

    return arg1;
}

long _spu_Fr(long arg0, long arg1) {
    _spu_t(2, D_8009AB78 << _spu_mem_mode_unitM);
    _spu_t(0);
    _spu_t(3, arg0, arg1);

    return arg1;
}

void _spu_FsetRXX(long arg0, u_long arg1, long arg2) {
    if (arg2 == 0) {
        D_8009AB7C[arg0] = arg1;
    } else {
        D_8009AB7C[arg0] = arg1 >> _spu_mem_mode_unitM;
    }
}

long _spu_FsetRXXa(long arg0, u_long arg1) {
    register long index asm("$6") = arg0;
    register u_long shifted asm("$7");
    register long shift asm("$2");
    u_long divisor;

    if (D_8009AB9C != 0) {
        divisor = D_8009ABA4;
        if ((arg1 % divisor) != 0) {
            arg1 += divisor;
            arg1 &= ~_spu_mem_mode_plus;
        }
    }

    shift = _spu_mem_mode_unitM;
    shifted = arg1 >> shift;

    if (index == -2) {
        goto ret_arg;
    }
    if (index != -1) {
        goto store;
    }
    return shifted & 0xFFFF;

ret_arg:
    return arg1;

store:
    {
        register long base asm("$4") = (long)D_8009AB7C;
        register long offset asm("$3") = index << 1;
        *(volatile u_short *)(offset + base) = shifted;
    }
    return arg1;
}

long _spu_FgetRXXa(long arg0, long arg1) {
    register long value asm("a0");
    long ret;

    value = D_8009AB7C_nonvolatile[arg0];
    if (arg1 != -1) {
        ret = value << _spu_mem_mode_unitM;
    } else {
        ret = value;
    }

    return ret;
}

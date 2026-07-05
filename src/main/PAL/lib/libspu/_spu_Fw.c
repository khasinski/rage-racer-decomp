#include "psyq/spu.h"

extern u16 D_8009AB78;
extern volatile u16 *D_8009AB7C;
extern u16 *D_8009AB7C_nonvolatile asm("D_8009AB7C");
extern s32 D_8009AB94;
extern s32 D_8009AB9C;
extern s32 _spu_mem_mode_unitM asm("D_8009ABA0");
extern u32 D_8009ABA4;
extern u32 _spu_mem_mode_plus asm("D_8009ABA8");

s32 _spu_Fw(s32 arg0, s32 arg1) {
    if (D_8009AB94 == 0) {
        _spu_t(2, D_8009AB78 << _spu_mem_mode_unitM);
        _spu_t(1);
        _spu_t(3, arg0, arg1);
    } else {
        _spu_writeByIO(arg0, arg1);
    }

    return arg1;
}

s32 _spu_Fr(s32 arg0, s32 arg1) {
    _spu_t(2, D_8009AB78 << _spu_mem_mode_unitM);
    _spu_t(0);
    _spu_t(3, arg0, arg1);

    return arg1;
}

void _spu_FsetRXX(s32 arg0, u32 arg1, s32 arg2) {
    if (arg2 == 0) {
        D_8009AB7C[arg0] = arg1;
    } else {
        D_8009AB7C[arg0] = arg1 >> _spu_mem_mode_unitM;
    }
}

s32 _spu_FsetRXXa(s32 arg0, u32 arg1) {
    register s32 index asm("$6") = arg0;
    register u32 shifted asm("$7");
    register s32 shift asm("$2");
    u32 divisor;

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
        register s32 base asm("$4") = (s32)D_8009AB7C;
        register s32 offset asm("$3") = index << 1;
        *(volatile u16 *)(offset + base) = shifted;
    }
    return arg1;
}

s32 _spu_FgetRXXa(s32 arg0, s32 arg1) {
    register s32 value asm("a0");
    s32 ret;

    value = D_8009AB7C_nonvolatile[arg0];
    if (arg1 != -1) {
        ret = value << _spu_mem_mode_unitM;
    } else {
        ret = value;
    }

    return ret;
}

#include "psyq/spu.h"

extern volatile u16 *D_8009AB7C;
extern s32 D_8009AB9C;
extern s32 _spu_mem_mode_unitM asm("D_8009ABA0");
extern u32 D_8009ABA4;
extern u32 _spu_mem_mode_plus asm("D_8009ABA8");

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

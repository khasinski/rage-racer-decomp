#include "psyq/spu.h"

extern u16 D_8009AB78;
extern s32 D_8009AB94;
extern s32 _spu_mem_mode_unitM asm("D_8009ABA0");

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

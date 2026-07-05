#include "psyq/spu.h"

extern u16 D_8009AB78;
extern s32 _spu_mem_mode_unitM asm("D_8009ABA0");

s32 _spu_Fr(s32 arg0, s32 arg1) {
    _spu_t(2, D_8009AB78 << _spu_mem_mode_unitM);
    _spu_t(0);
    _spu_t(3, arg0, arg1);

    return arg1;
}

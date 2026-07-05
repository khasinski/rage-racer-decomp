#include "psyq/spu.h"

extern u16 *D_8009AB7C;
extern s32 _spu_mem_mode_unitM asm("D_8009ABA0");

s32 _spu_FgetRXXa(s32 arg0, s32 arg1) {
    register s32 value asm("a0");
    s32 ret;

    value = D_8009AB7C[arg0];
    if (arg1 != -1) {
        ret = value << _spu_mem_mode_unitM;
    } else {
        ret = value;
    }

    return ret;
}

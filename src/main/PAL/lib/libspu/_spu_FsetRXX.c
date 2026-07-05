#include "psyq/spu.h"

extern volatile u16 *D_8009AB7C;
extern s32 _spu_mem_mode_unitM asm("D_8009ABA0");

void _spu_FsetRXX(s32 arg0, u32 arg1, s32 arg2) {
    if (arg2 == 0) {
        D_8009AB7C[arg0] = arg1;
    } else {
        D_8009AB7C[arg0] = arg1 >> _spu_mem_mode_unitM;
    }
}

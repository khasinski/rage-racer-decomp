#include "psyq/spu.h"

extern s32 _spu_mem_mode_unitM asm("D_8009ABA0");
extern s32 _spu_AllocBlockNum asm("D_8009ABD4");
extern s32 _spu_AllocLastNum asm("D_8009ABD8");
extern SpuMallocEntry *_spu_memList asm("D_8009ABDC");

s32 SpuInitMalloc(s32 arg0, u32 *arg1) asm("func_80079B60");

s32 SpuInitMalloc(s32 arg0, u32 *arg1) {
    s32 ret = arg0;
    u32 size;
    s32 shift;

    if (ret > 0) {
        shift = _spu_mem_mode_unitM;
        arg1[0] = 0x40001010;
        _spu_memList = (SpuMallocEntry *)arg1;
        _spu_AllocLastNum = 0;
        _spu_AllocBlockNum = ret;
        size = 0x10000 << shift;
        size -= 0x1010;
        arg1[1] = size;

        return ret;
    }

    return 0;
}

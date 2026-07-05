#include "psyq/spu.h"

extern s32 D_8009A71C;
extern s32 D_8009A720;
extern s32 _spu_mem_mode_unitM asm("D_8009ABA0");
extern s32 _spu_mem_mode_plus asm("D_8009ABA8");
extern s32 _spu_AllocBlockNum asm("D_8009ABD4");
extern s32 _spu_AllocLastNum asm("D_8009ABD8");
extern SpuMallocEntry *_spu_memList asm("D_8009ABDC");

void _spu_gcSPU(void) asm("func_80079E7C");

#define BLK(i) (&_spu_memList[(i)])
#define BLK_ADDR(i) (BLK(i)->addr & 0x0FFFFFFF)

s32 SpuMalloc(s32 size) asm("func_80079BB4");

s32 SpuMalloc(s32 size) {
    s32 var_a0;
    s32 var_s2;
    s32 var_s3;
    s32 var_v0;
    s32 temp_s1;
    s32 i;

    i = 0;
    var_s2 = -1;

    if (D_8009A71C == 0) {
        var_s3 = 0;
    } else {
        var_s3 = (0x10000 - D_8009A720) << _spu_mem_mode_unitM;
    }

    var_a0 = size;
    if (size & ~_spu_mem_mode_plus) {
        var_a0 += _spu_mem_mode_plus;
    }

    size = var_a0;
    size >>= _spu_mem_mode_unitM;
    size <<= _spu_mem_mode_unitM;

    if (BLK(0)->addr & 0x40000000) {
        var_s2 = 0;
    } else {
        _spu_gcSPU();

        for (; i < _spu_AllocBlockNum; i++) {
            if ((BLK(i)->addr & 0x40000000) || ((BLK(i)->addr & 0x80000000) && (BLK(i)->size >= size))) {
                var_s2 = i;
                break;
            }
        }
    }

    if (var_s2 == -1) {
        return -1;
    }

    if (BLK(var_s2)->addr & 0x40000000) {
        if ((var_s2 < _spu_AllocBlockNum) && ((BLK(var_s2)->size - var_s3) >= size)) {
            s32 next = var_s2 + 1;

            BLK(next)->addr = ((*(volatile s32 *)&BLK(var_s2)->addr & 0x0FFFFFFF) + size) | 0x40000000;
            BLK(next)->size = BLK(var_s2)->size - size;

            BLK(var_s2)->addr &= 0x0FFFFFFF;
            BLK(var_s2)->size = size;

            _spu_AllocLastNum = next;
            _spu_gcSPU();

            return BLK(var_s2)->addr;
        }
    } else {
        if ((size < BLK(var_s2)->size) && (_spu_AllocLastNum < _spu_AllocBlockNum)) {
            s32 t_addr;
            s32 t_size;
            s32 addr = BLK(var_s2)->addr + size;
            s32 split_size = BLK(var_s2)->size - size;

            t_addr = BLK(_spu_AllocLastNum)->addr;
            t_size = BLK(_spu_AllocLastNum)->size;

            BLK(_spu_AllocLastNum)->addr = addr | 0x80000000;
            BLK(_spu_AllocLastNum)->size = split_size;

            BLK(_spu_AllocLastNum + 1)->addr = t_addr;
            BLK(_spu_AllocLastNum + 1)->size = t_size;

            _spu_AllocLastNum++;
        }

        BLK(var_s2)->size = size;
        BLK(var_s2)->addr = BLK_ADDR(var_s2);
        _spu_gcSPU();

        return BLK(var_s2)->addr;
    }

    return -1;
}

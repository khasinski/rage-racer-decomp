#include "psyq/spu.h"

extern s32 _spu_mem_mode_unitM asm("D_8009ABA0");
extern s32 D_8009A71C;
extern s32 D_8009A720;
extern s32 _spu_mem_mode_plus asm("D_8009ABA8");
extern s32 _spu_AllocBlockNum asm("D_8009ABD4");
extern s32 _spu_AllocLastNum asm("D_8009ABD8");
extern SpuMallocEntry *_spu_memList asm("D_8009ABDC");

#define BLK(i) (&_spu_memList[(i)])
#define BLK_ADDR(i) (BLK(i)->addr & 0x0FFFFFFF)
#define BLK_END(i) ((BLK(i)->addr & 0x0FFFFFFF) + BLK(i)->size)
#define BLK_IS_FREE(i) (BLK(i)->addr & 0x80000000)
#define BLK_IS_END(i) (BLK(i)->addr & 0x40000000)

s32 SpuInitMalloc(s32 arg0, u32 *arg1) asm("func_80079B60");
s32 SpuMalloc(s32 size) asm("func_80079BB4");
void _spu_gcSPU(void) asm("func_80079E7C");
void SpuFree(u32 arg0) asm("func_8007A17C");

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

void _spu_gcSPU(void) {
    s32 i;
    s32 j;

    for (i = 0; i <= _spu_AllocLastNum;) {
        if (BLK_IS_FREE(i)) {
            SpuMallocEntry *scan;

            j = i + 1;
            scan = BLK(j);
            while (1) {
                s32 is_not_empty = scan->addr != 0x2FFFFFFF;
                scan++;
                if (is_not_empty) {
                    break;
                }
                j++;
            }

            if (BLK_IS_FREE(j) && (BLK_ADDR(j) == BLK_END(i))) {
                BLK(j)->addr = 0x2FFFFFFF;
                BLK(i)->size += BLK(j)->size;
                continue;
            }
        }
        i++;
    }

    for (i = 0; i <= _spu_AllocLastNum; i++) {
        if (BLK(i)->size == 0) {
            BLK(i)->addr = 0x2FFFFFFF;
        }
    }

    for (i = 0; i <= _spu_AllocLastNum; i++) {
        if (BLK_IS_END(i)) {
            break;
        }

        for (j = i + 1; j <= _spu_AllocLastNum; j++) {
            if (BLK_IS_END(j)) {
                break;
            }

            if (BLK_ADDR(j) < BLK_ADDR(i)) {
                u32 swap_addr = BLK(i)->addr;
                u32 swap_size = BLK(i)->size;
                BLK(i)->addr = BLK(j)->addr;
                BLK(i)->size = BLK(j)->size;
                BLK(j)->addr = swap_addr;
                BLK(j)->size = swap_size;
            }
        }
    }

    for (i = 0; i <= _spu_AllocLastNum; i++) {
        if (BLK_IS_END(i)) {
            break;
        }

        if (BLK(i)->addr == 0x2FFFFFFF) {
            BLK(i)->addr = BLK(_spu_AllocLastNum)->addr;
            BLK(i)->size = BLK(_spu_AllocLastNum)->size;
            _spu_AllocLastNum = i;
            break;
        }
    }

    for (i = _spu_AllocLastNum - 1; i >= 0; i--) {
        if (!BLK_IS_FREE(i)) {
            break;
        }
        BLK(i)->addr = BLK_ADDR(i) | 0x40000000;
        BLK(i)->size += BLK(_spu_AllocLastNum)->size;
        _spu_AllocLastNum = i;
    }
}

void SpuFree(u32 arg0) {
    s32 cur_block_num;
    SpuMallocEntry *cur_mem;
    u32 temp;
    u32 cur_addr;
    s32 mask4;
    s32 block_num;
    u8 pad[2];

    cur_block_num = 0;
    if (_spu_AllocBlockNum > 0) {
        mask4 = 0x40000000;
        temp = arg0 | 0x80000000;
        block_num = _spu_AllocBlockNum;
        cur_mem = _spu_memList;
        while (1) {
            cur_addr = cur_mem->addr;
            if (cur_addr & mask4) {
                break;
            }
            if (cur_addr == arg0) {
                cur_mem->addr = temp;
                break;
            }
            cur_mem++;
            cur_block_num++;
            if (cur_block_num >= block_num) {
                break;
            }
        }
    }
    _spu_gcSPU();
}

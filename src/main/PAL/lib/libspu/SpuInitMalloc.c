#include "psyq/spu.h"

extern long _spu_mem_mode_plus;

#define BLK(i) (&_spu_memList[(i)])
#define BLK_ADDR(i) (BLK(i)->addr & 0x0FFFFFFF)
#define BLK_END(i) ((BLK(i)->addr & 0x0FFFFFFF) + BLK(i)->size)
#define BLK_IS_FREE(i) (BLK(i)->addr & 0x80000000)
#define BLK_IS_END(i) (BLK(i)->addr & 0x40000000)

long SpuInitMalloc(long num, u_long *memlist) {
    long ret = num;
    u_long size;
    long shift;

    if (ret > 0) {
        shift = _spu_mem_mode_unitM;
        memlist[0] = 0x40001010;
        _spu_memList = (SpuMallocEntry *)memlist;
        _spu_AllocLastNum = 0;
        _spu_AllocBlockNum = ret;
        size = 0x10000 << shift;
        size -= 0x1010;
        memlist[1] = size;

        return ret;
    }

    return 0;
}

long SpuMalloc(long size) {
    long var_a0;
    long var_s2;
    long var_s3;
    long var_v0;
    long temp_s1;
    long i;

    i = 0;
    var_s2 = -1;

    if (g_SpuRevReserveWa == 0) {
        var_s3 = 0;
    } else {
        var_s3 = (0x10000 - g_SpuRevWorkAreaAddr) << _spu_mem_mode_unitM;
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
            long next = var_s2 + 1;

            BLK(next)->addr = ((*(volatile long *)&BLK(var_s2)->addr & 0x0FFFFFFF) + size) | 0x40000000;
            BLK(next)->size = BLK(var_s2)->size - size;

            BLK(var_s2)->addr &= 0x0FFFFFFF;
            BLK(var_s2)->size = size;

            _spu_AllocLastNum = next;
            _spu_gcSPU();

            return BLK(var_s2)->addr;
        }
    } else {
        if ((size < BLK(var_s2)->size) && (_spu_AllocLastNum < _spu_AllocBlockNum)) {
            long t_addr;
            long t_size;
            long addr = BLK(var_s2)->addr + size;
            long split_size = BLK(var_s2)->size - size;

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
    long i;
    long j;

    for (i = 0; i <= _spu_AllocLastNum;) {
        if (BLK_IS_FREE(i)) {
            SpuMallocEntry *scan;

            j = i + 1;
            scan = BLK(j);
            while (1) {
                long is_not_empty = scan->addr != 0x2FFFFFFF;
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
                u_long swap_addr = BLK(i)->addr;
                u_long swap_size = BLK(i)->size;
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

void SpuFree(u_long arg0) {
    long cur_block_num;
    SpuMallocEntry *cur_mem;
    u_long temp;
    u_long cur_addr;
    long mask4;
    long block_num;
    u_char pad[2];

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

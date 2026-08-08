#include "psyq/spu.h"
#include "psyq/spu_internal.h"


#define BLK(i) (&_spu_memList[(i)])
#define BLK_ADDR(i) (BLK(i)->addr & SPU_BLOCK_ADDR)
#define BLK_END(i) ((BLK(i)->addr & SPU_BLOCK_ADDR) + BLK(i)->size)
#define BLK_IS_FREE(i) (BLK(i)->addr & SPU_BLOCK_FREE)
#define BLK_IS_END(i) (BLK(i)->addr & SPU_BLOCK_END)

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
    long aligned;
    long block;
    long reverbReserve;
    long i;

    i = 0;
    block = -1;

    if (g_SpuRevReserveWa == 0) {
        reverbReserve = 0;
    } else {
        reverbReserve = (0x10000 - g_SpuRevWorkAreaAddr) << _spu_mem_mode_unitM;
    }

    aligned = size;
    if (size & ~_spu_mem_mode_plus) {
        aligned += _spu_mem_mode_plus;
    }

    size = aligned;
    size >>= _spu_mem_mode_unitM;
    size <<= _spu_mem_mode_unitM;

    if (BLK(0)->addr & SPU_BLOCK_END) {
        block = 0;
    } else {
        _spu_gcSPU();

        for (; i < _spu_AllocBlockNum; i++) {
            if ((BLK(i)->addr & SPU_BLOCK_END) || ((BLK(i)->addr & SPU_BLOCK_FREE) && (BLK(i)->size >= size))) {
                block = i;
                break;
            }
        }
    }

    if (block == -1) {
        return -1;
    }

    if (BLK(block)->addr & SPU_BLOCK_END) {
        if ((block < _spu_AllocBlockNum) && ((BLK(block)->size - reverbReserve) >= size)) {
            long next = block + 1;

            BLK(next)->addr = ((*(volatile long *)&BLK(block)->addr & SPU_BLOCK_ADDR) + size) | SPU_BLOCK_END;
            BLK(next)->size = BLK(block)->size - size;

            BLK(block)->addr &= SPU_BLOCK_ADDR;
            BLK(block)->size = size;

            _spu_AllocLastNum = next;
            _spu_gcSPU();

            return BLK(block)->addr;
        }
    } else {
        if ((size < BLK(block)->size) && (_spu_AllocLastNum < _spu_AllocBlockNum)) {
            long t_addr;
            long t_size;
            long addr = BLK(block)->addr + size;
            long split_size = BLK(block)->size - size;

            t_addr = BLK(_spu_AllocLastNum)->addr;
            t_size = BLK(_spu_AllocLastNum)->size;

            BLK(_spu_AllocLastNum)->addr = addr | SPU_BLOCK_FREE;
            BLK(_spu_AllocLastNum)->size = split_size;

            BLK(_spu_AllocLastNum + 1)->addr = t_addr;
            BLK(_spu_AllocLastNum + 1)->size = t_size;

            _spu_AllocLastNum++;
        }

        BLK(block)->size = size;
        BLK(block)->addr = BLK_ADDR(block);
        _spu_gcSPU();

        return BLK(block)->addr;
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
                long is_not_empty = scan->addr != SPU_BLOCK_EMPTY;
                scan++;
                if (is_not_empty) {
                    break;
                }
                j++;
            }

            if (BLK_IS_FREE(j) && (BLK_ADDR(j) == BLK_END(i))) {
                BLK(j)->addr = SPU_BLOCK_EMPTY;
                BLK(i)->size += BLK(j)->size;
                continue;
            }
        }
        i++;
    }

    for (i = 0; i <= _spu_AllocLastNum; i++) {
        if (BLK(i)->size == 0) {
            BLK(i)->addr = SPU_BLOCK_EMPTY;
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

        if (BLK(i)->addr == SPU_BLOCK_EMPTY) {
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
        BLK(i)->addr = BLK_ADDR(i) | SPU_BLOCK_END;
        BLK(i)->size += BLK(_spu_AllocLastNum)->size;
        _spu_AllocLastNum = i;
    }
}

void SpuFree(u_long addr) {
    long cur_block_num;
    SpuMallocEntry *cur_mem;
    u_long temp;
    u_long cur_addr;
    long endBit;
    long block_num;
    u_char pad[2];

    cur_block_num = 0;
    if (_spu_AllocBlockNum > 0) {
        endBit = SPU_BLOCK_END;
        temp = addr | SPU_BLOCK_FREE;
        block_num = _spu_AllocBlockNum;
        cur_mem = _spu_memList;
        while (1) {
            cur_addr = cur_mem->addr;
            if (cur_addr & endBit) {
                break;
            }
            if (cur_addr == addr) {
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

#include "psyq/spu.h"

extern s32 _spu_AllocLastNum asm("D_8009ABD8");
extern SpuMallocEntry *_spu_memList asm("D_8009ABDC");

#define BLK(i) (&_spu_memList[(i)])
#define BLK_ADDR(i) (BLK(i)->addr & 0x0FFFFFFF)
#define BLK_END(i) ((BLK(i)->addr & 0x0FFFFFFF) + BLK(i)->size)
#define BLK_IS_FREE(i) (BLK(i)->addr & 0x80000000)
#define BLK_IS_END(i) (BLK(i)->addr & 0x40000000)

void _spu_gcSPU(void) asm("func_80079E7C");

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

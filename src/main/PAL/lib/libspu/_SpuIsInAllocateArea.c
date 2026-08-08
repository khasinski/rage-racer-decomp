#include "psyq/spu.h"
#include "psyq/spu_internal.h"

long _SpuIsInAllocateArea(u_long addr) {
    SpuMallocEntry *entry;
    u_long header;
    u_long freeBit;
    u_long endBit;
    u_long addrMask;

    freeBit = SPU_BLOCK_FREE;
    endBit = SPU_BLOCK_END;
    addrMask = SPU_BLOCK_ADDR;

    entry = _spu_memList;
    while (1) {
        header = entry->addr;
        if (header & freeBit) {
            entry += 1;
            continue;
        }
        if (!(header & endBit)) {
            if ((header & addrMask) < addr) {
                if (addr >= (u_long)((header & addrMask) + entry->size)) {
                    entry += 1;
                    continue;
                }
                return 1;
            } else {
                return 1;
            }
        }
        break;
    }
    return 0;
}

long _SpuIsInAllocateArea_(u_long addr) {
    SpuMallocEntry *entry;
    u_long header;
    u_long freeBit;
    u_long endBit;
    u_long addrMask;

    freeBit = SPU_BLOCK_FREE;
    endBit = SPU_BLOCK_END;
    addrMask = SPU_BLOCK_ADDR;

    entry = _spu_memList;
    addr <<= _spu_mem_mode_unitM;
    while (1) {
        header = entry->addr;
        if (header & freeBit) {
            entry += 1;
            continue;
        }
        if (!(header & endBit)) {
            if ((header & addrMask) < addr) {
                if (addr >= (u_long)((header & addrMask) + entry->size)) {
                    entry += 1;
                    continue;
                }
                return 1;
            } else {
                return 1;
            }
        }
        break;
    }
    return 0;
}

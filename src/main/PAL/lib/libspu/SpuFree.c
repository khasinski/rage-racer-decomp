#include "psyq/spu.h"

extern s32 _spu_AllocBlockNum asm("D_8009ABD4");
extern SpuMallocEntry *_spu_memList asm("D_8009ABDC");

void _spu_gcSPU(void) asm("func_80079E7C");

void SpuFree(u32 arg0) asm("func_8007A17C");

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

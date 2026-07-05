#include "psyq/spu.h"
#include "psyq/kernel.h"

extern s32 D_8009A768;
extern s32 D_8009A770[];
extern s32 D_8009AB94;
extern s32 _spu_mem_mode_unitM asm("D_8009ABA0");
extern volatile s32 D_8009ABB0;
extern s32 D_8009ABE0[];

s32 SpuClearReverbWorkArea(u32 mode) {
    volatile s32 callback;
    s32 old_transmode;
    u32 size;
    u32 addr;
    s32 chunk_active;
    u32 chunk_size;
    s32 restored_transmode;

    callback = 0;
    restored_transmode = 0;
    if ((mode >= 0xA) || (_SpuIsInAllocateArea_(D_8009ABE0[mode]) != 0)) {
        return -1;
    }

    if (mode == 0) {
        size = 0x10 << _spu_mem_mode_unitM;
        addr = 0xFFF0 << _spu_mem_mode_unitM;
    } else {
        size = (0x10000 - D_8009ABE0[mode]) << _spu_mem_mode_unitM;
        addr = D_8009ABE0[mode] << _spu_mem_mode_unitM;
    }

    old_transmode = D_8009AB94;
    if (D_8009AB94 == 1) {
        D_8009AB94 = 0;
        restored_transmode = 1;
    }

    chunk_active = 1;
    if (D_8009ABB0 != 0) {
        callback = D_8009ABB0;
        D_8009ABB0 = 0;
    }

    do {
        chunk_size = size;
        if (size >= 0x401) {
            chunk_size = 0x400;
        } else {
            chunk_active = 0;
        }

        _spu_t(2, addr);
        _spu_t(1);
        _spu_t(3, D_8009A770, chunk_size);
        WaitEvent(D_8009A768);
        size -= 0x400;
        addr += 0x400;
    } while (chunk_active != 0);

    if (restored_transmode != 0) {
        D_8009AB94 = old_transmode;
    }
    if (callback != 0) {
        D_8009ABB0 = callback;
    }
    return 0;
}

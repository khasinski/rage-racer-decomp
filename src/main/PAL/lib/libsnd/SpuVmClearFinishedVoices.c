#include "common.h"

extern u8 D_801E42F8;
extern u8 D_8009E0D3[];
extern u8 D_8009E0BC[];
extern u8 *D_8009A588;

void SpuVmClearFinishedVoices(void) asm("func_80075B78");

void SpuVmClearFinishedVoices(void) {
    register s32 i asm("$4");
    register s32 next asm("$2");
    register s32 flag asm("$5");
    register s32 offset asm("$3");
    register s32 bound asm("$3");
    register u8 *ptr asm("$2");

    i = 0;
    if (D_801E42F8 == 0) {
        return;
    }

    flag = 2;
    __asm__ volatile("" : "=r"(i) : "0"(i));
    next = i << 16;

loop:
    next >>= 16;
    offset = next * 0x34;
    if (D_8009E0D3[offset] == flag) {
        offset = ((u8)i) * 0x34;
        D_8009E0D3[offset] = 0;
        ptr = D_8009A588;
        *(u16 *)(D_8009E0BC + offset) = 0;
        *(u16 *)(ptr + 0x194) = 0;
        *(u16 *)(ptr + 0x196) = 0;
    }

    next = i + 1;
    i = next;
    next <<= 16;
    bound = D_801E42F8;
    next >>= 16;
    next = next < bound;
    if (next) {
        next = i << 16;
        goto loop;
    }
}

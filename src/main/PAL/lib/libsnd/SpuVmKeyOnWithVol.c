#include "common.h"

extern u8 D_801E4BDF;
extern u8 D_801E42F8;
extern u8 D_801E4BEA;
extern u8 D_8009E0D3[];
extern u8 D_8009E0BC[];
extern u8 *D_8009A588;

u8 SpuVmAlloc(s32 arg0) asm("func_800739E8");
void SpuVmKeyOnCore(s32 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4) asm("func_80074818");

void SpuVmKeyOnWithVol(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_80075AE0");
void SpuVmClearFinishedVoices(void) asm("func_80075B78");
void SpuVmKeyOnWithDefaultVol(s32 arg0, s32 arg1) asm("func_80075C30");

void SpuVmKeyOnWithVol(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 ret;

    D_801E4BDF = 0x7F;
    ret = SpuVmAlloc(0xFF) & 0xFF;
    *(u16 *)&D_801E4BEA = ret;

    if (ret < D_801E42F8) {
        SpuVmKeyOnCore(D_801E4BEA, arg0, arg1, arg2, arg3);
    }
}

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

void SpuVmKeyOnWithDefaultVol(s32 arg0, s32 arg1) {
    s32 ret;

    D_801E4BDF = 0x7F;
    ret = SpuVmAlloc(0xFF) & 0xFF;
    *(u16 *)&D_801E4BEA = ret;

    if (ret < D_801E42F8) {
        SpuVmKeyOnCore(D_801E4BEA, arg0, arg1, 0x80FF, 0x5FC8);
    }
}

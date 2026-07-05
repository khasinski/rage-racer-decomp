#include "common.h"

extern u8 D_801E4BDF;
extern u8 D_801E42F8;
extern u8 D_801E4BEA;

u8 SpuVmAlloc(s32 arg0) asm("func_800739E8");
void SpuVmKeyOnCore(s32 arg0, u16 arg1, u16 arg2, s32 arg3, s32 arg4) asm("func_80074818");

void SpuVmKeyOnWithDefaultVol(s32 arg0, s32 arg1) asm("func_80075C30");

void SpuVmKeyOnWithDefaultVol(s32 arg0, s32 arg1) {
    s32 ret;

    D_801E4BDF = 0x7F;
    ret = SpuVmAlloc(0xFF) & 0xFF;
    *(u16 *)&D_801E4BEA = ret;

    if (ret < D_801E42F8) {
        SpuVmKeyOnCore(D_801E4BEA, arg0, arg1, 0x80FF, 0x5FC8);
    }
}

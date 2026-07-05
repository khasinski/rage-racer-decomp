#include "common.h"

extern u8 *D_801E79CC[];
extern s16 D_801E4BE6;

s16 SpuVmGetSeqVolRight(s32 arg0) asm("func_80076E88");

s16 SpuVmGetSeqVolRight(s32 arg0) {
    s32 index;
    register s32 offset asm("$2");
    register u8 *ptr asm("$3");

    offset = (arg0 & 0xFF) << 2;
    ptr = *(u8 **)((u8 *)D_801E79CC + offset);
    D_801E4BE6 = arg0;
    index = (arg0 & 0xFF00) >> 8;

    return *(s16 *)(ptr + (index * 0xAC) + 0x76);
}

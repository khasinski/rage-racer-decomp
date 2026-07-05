#include "common.h"

extern u8 *D_801E79CC[];
extern s16 D_801E4BE6;

s32 SpuVmGetSeqVol(s32 arg0, s16 *arg1, s16 *arg2) asm("func_80076DCC");

s32 SpuVmGetSeqVol(s32 arg0, s16 *arg1, s16 *arg2) {
    s32 index;
    register s32 offset asm("$2");
    register u8 *base asm("$3");
    register u8 *ptr asm("$2");
    register s16 *status asm("$7");

    status = &D_801E4BE6;
    offset = (arg0 & 0xFF) << 2;
    base = *(u8 **)((u8 *)D_801E79CC + offset);
    *status = arg0;
    index = (arg0 & 0xFF00) >> 8;

    ptr = (u8 *)((index * 0xAC) + (s32)base);
    *arg1 = *(u16 *)(ptr + 0x74);
    *arg2 = *(u16 *)(ptr + 0x76);
    return *status;
}

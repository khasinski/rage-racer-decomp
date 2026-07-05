#include "common.h"

void SsUnpackAdsr(u32 arg0, u32 arg1, u16 *out) asm("func_80070E28");

void SsUnpackAdsr(u32 arg0, u32 arg1, u16 *out) {
    out[5] = arg0 & 0x8000;
    out[6] = arg1 & 0x8000;
    out[8] = arg1 & 0x4000;
    out[7] = arg1 & 0x20;
    out[0] = ((arg0 & 0xFFFF) >> 8) & 0x7F;
    out[1] = ((arg0 & 0xFFFF) >> 4) & 0xF;
    out[2] = arg0 & 0xF;
    out[3] = (arg1 >> 6) & 0x7F;
    out[4] = arg1 & 0x1F;
}

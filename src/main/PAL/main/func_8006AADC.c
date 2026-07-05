#include "common.h"
#include "psyq/cd.h"

s32 CdPosToInt_Local(CdlLOC *arg0) {
    register s32 v0 asm("$2");
    register s32 v1 asm("$3") = arg0->minute;
    register s32 a0 asm("$4");
    register s32 a1 asm("$5");
    register s32 a2 asm("$6") = arg0->second;

    a1 = (u32) v1 >> 4;
    v0 = a1 << 2;
    v0 += a1;
    v0 <<= 1;
    v1 &= 0xF;
    v0 += v1;
    a1 = v0 << 4;
    a1 -= v0;
    a1 <<= 2;

    v1 = (u32) a2 >> 4;
    v0 = v1 << 2;
    v0 += v1;
    v0 <<= 1;
    a2 &= 0xF;
    v0 += a2;
    a1 += v0;
    v1 = a1 << 2;
    v1 += a1;
    v0 = v1 << 4;

    a1 = arg0->sector;
    v0 -= v1;
    a0 = (u32) a1 >> 4;
    v1 = a0 << 2;
    v1 += a0;
    v1 <<= 1;
    a1 &= 0xF;
    v1 += a1;
    v0 += v1;

    return v0 - 150;
}

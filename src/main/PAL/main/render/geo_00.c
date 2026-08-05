#include "common.h"
#include "psyq/gte.h"

extern s16 D_80092B08[];
extern s16 D_80093308[];
extern s16 D_80093B08[];
extern s16 g_SinTable[] asm("D_80094308");

s32 rsin(s32 arg0) asm("func_80068568");
s32 rsinCore(s32 arg0);
s32 rcos(s32 arg0) asm("func_80068634");

/*
 * PSY-Q 3.5 libgte object geo_00.o (LIBGTE.A): the fixed-point sine/cosine
 * helpers rsin (rsin), rsinCore (rsinCore) and rcos
 * (rcos) that read the 0x80094308 sine table.  Byte-matched against
 * geo_00.o (rsin anchor); see docs/names.md section 25.
 */

s32 rsin(s32 arg0) {
    if (arg0 < 0) {
        return -rsinCore(-arg0 & 0xFFF);
    }

    return rsinCore(arg0 & 0xFFF);
}

s32 rsinCore(s32 arg0) {
    if (arg0 < 0x801) {
        if (arg0 < 0x401) {
            return g_SinTable[arg0];
        }

        return g_SinTable[0x800 - arg0];
    }

    if (arg0 < 0xC01) {
        return -D_80093308[arg0];
    }

    return -g_SinTable[0x1000 - arg0];
}

s32 rcos(s32 arg0) {
    if (arg0 < 0) {
        arg0 = -arg0;
    }

    arg0 &= 0xFFF;

    if (arg0 < 0x801) {
        if (arg0 < 0x401) {
            return g_SinTable[0x400 - arg0];
        }

        return -D_80093B08[arg0];
    }

    if (arg0 < 0xC01) {
        return -g_SinTable[0xC00 - arg0];
    }

    return D_80092B08[arg0];
}

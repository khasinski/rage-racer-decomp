#include "common.h"
#include "psyq/gte_macros.h"

s32 func_80069C7C(s32 x) {
    s32 r;
    gte_lzc(r, x);
    return r;
}

/* lone-nop alignment padding (func_80069C94) that 8-byte-aligns the next TU */
const u32 func_80069C94[1] __attribute__((section(".text"))) = { 0 };

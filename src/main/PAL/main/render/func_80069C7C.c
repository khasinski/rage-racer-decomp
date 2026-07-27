#include "common.h"
#include "psyq/gte_macros.h"

s32 func_80069C7C(s32 x) {
    s32 r;
    gte_lzc(r, x);
    return r;
}

/* lone-nop alignment padding (func_80069C94) that 8-byte-aligns the next TU */
const u32 func_80069C94[1] __attribute__((section(".text"))) = { 0 };

s32 NormalClip(s32 arg0, s32 arg1, s32 arg2) asm("func_80069C98");
s32 NormalClip(s32 arg0, s32 arg1, s32 arg2) {
    register s32 ret asm("$2");
    gte_mtc2(arg0, 12);
    gte_mtc2(arg2, 14);
    gte_mtc2(arg1, 13);
    gte_nop();
    gte_nop();
    gte_nclip();
    gte_stopz(ret);
    return ret;
}

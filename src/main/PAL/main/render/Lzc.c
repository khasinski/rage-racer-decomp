#include "common.h"
#include "psyq/gte_macros.h"

s32 Lzc(s32 data);
s32 Lzc(s32 x) {
    s32 r;
    gte_lzc(r, x);
    return r;
}

/* lone-nop alignment padding (func_80069C94) that 8-byte-aligns the next TU */
const u32 func_80069C94[1] __attribute__((section(".text"))) = { 0 };

s32 NormalClip(s32 sxy0, s32 sxy1, s32 sxy2) {
    s32 ret;
    gte_mtc2(sxy0, 12);
    gte_mtc2(sxy2, 14);
    gte_mtc2(sxy1, 13);
    gte_nop();
    gte_nop();
    gte_nclip();
    gte_stopz(ret);
    return ret;
}

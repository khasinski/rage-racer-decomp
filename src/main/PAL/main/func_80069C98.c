#include "common.h"
#include "psyq/gte_macros.h"

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

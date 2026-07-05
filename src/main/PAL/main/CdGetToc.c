#include "common.h"

s32 CdGetToc2(s32 arg0, s32 arg1) asm("func_8006A0AC");

s32 CdGetToc(s32 arg0) asm("func_8006A088");
s32 CdGetToc(s32 arg0) {
    return CdGetToc2(1, arg0);
}

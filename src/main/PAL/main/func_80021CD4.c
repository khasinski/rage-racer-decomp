#include "common.h"

extern char D_80010EB4[];

void func_800632F0(void *dst, void *fmt, s32 arg0, s32 arg1, s32 arg2);

void *func_80021CD4(void *dst, s32 value) {
    s32 minutes = value / 60000;
    s32 ticks = value / 1000;
    s32 seconds = ticks - (minutes * 60);
    s32 fraction = value - (ticks * 1000);

    func_800632F0(dst, D_80010EB4, minutes, seconds, fraction);
    return dst;
}

#include "common.h"

s32 SsVabOpenHeadWithMode(void *arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_80072CB4");

s32 SsVabOpenHead(void *arg0, s32 arg1) asm("func_80072C18");
s32 SsVabOpenHeadSticky(void *arg0, s32 arg1, s32 arg2) asm("func_80072C4C");
s32 SsVabFakeHead(void *arg0, s32 arg1, s32 arg2) asm("func_80072C80");

s32 SsVabOpenHead(void *arg0, s32 arg1) {
    s32 ret;

    ret = SsVabOpenHeadWithMode(arg0, (s16)arg1, 0, 0);
    ret = (s16)ret;
    asm volatile("" : "=r"(ret) : "0"(ret));
    return ret;
}

s32 SsVabOpenHeadSticky(void *arg0, s32 arg1, s32 arg2) {
    s32 ret;

    ret = SsVabOpenHeadWithMode(arg0, (s16)arg1, 1, arg2);
    ret = (s16)ret;
    asm volatile("" : "=r"(ret) : "0"(ret));
    return ret;
}

s32 SsVabFakeHead(void *arg0, s32 arg1, s32 arg2) {
    s32 ret;

    ret = SsVabOpenHeadWithMode(arg0, (s16)arg1, 1, arg2);
    ret = (s16)ret;
    asm volatile("" : "=r"(ret) : "0"(ret));
    return ret;
}

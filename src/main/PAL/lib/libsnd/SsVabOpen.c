#include "common.h"

extern void *D_801F180C[];

s32 SsVabOpenHead(void *arg0, s32 arg1) asm("func_80072C18");
s32 SsVabOpenHeadWithMode(void *arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_80072CB4");
s32 SsVabOpenHeadSticky(void *arg0, s32 arg1, s32 arg2) asm("func_80072C4C");
s32 SsVabFakeHead(void *arg0, s32 arg1, s32 arg2) asm("func_80072C80");
s32 SsVabTransBody(void *arg0, s32 arg1) asm("func_800730BC");

s32 SsVabOpen(void *arg0) asm("func_80072BC0");

s32 SsVabOpen(void *arg0) {
    register s32 ret asm("$3");
    register s32 index asm("$5");
    register s32 temp asm("$2");
    register s32 out asm("$2");

    ret = SsVabOpenHead(arg0, -1);
    temp = ret << 16;
    index = temp >> 16;
    if (index != -1) {
        temp = index << 2;
        ret = SsVabTransBody(*(void **)((u8 *)D_801F180C + temp), index);
    }

    asm volatile(
        "sll %0,%1,16\n\t"
        "sra %0,%0,16"
        : "=r"(out)
        : "r"(ret));
    return out;
}

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

INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libsnd/SsVabOpen", SsVabOpenHeadWithMode);

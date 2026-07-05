#include "common.h"

extern void *D_801F180C[];

s32 SsVabOpenHead(void *arg0, s32 arg1) asm("func_80072C18");
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

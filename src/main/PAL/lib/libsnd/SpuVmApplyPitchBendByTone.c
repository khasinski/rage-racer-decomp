#include "common.h"

extern volatile u8 D_801E42F8;
extern s16 D_801E4BE6;

void SpuVmVSetUp(s32 arg0, s32 arg1) asm("func_80073314");
s32 SpuVmApplyPitchBendToVoice(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) asm("func_80075CB0");

s32 SpuVmApplyPitchBendByTone(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_80075EB4");

s32 SpuVmApplyPitchBendByTone(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    register s32 voice asm("$18");
    register s32 x asm("$21");
    register s32 y asm("$20");
    register s32 extra asm("$19");
    register s32 i asm("$16");
    register s32 sum asm("$17");
    register s32 bound asm("$3");
    register s32 store_voice asm("$2");
    register s32 tmp asm("$2");
    register s32 next asm("$2");
    s32 stack_pad[2];
    s32 call_x;
    s32 call_y;

    voice = arg0;
    tmp = arg1;
    call_x = (s16)tmp;
    call_y = (s16)arg2;
    x = tmp;
    y = arg2;
    extra = arg3;

    SpuVmVSetUp(call_x, call_y);
    __asm__ volatile("" : "=m"(stack_pad[0]), "=m"(stack_pad[1]) : "m"(stack_pad[0]), "m"(stack_pad[1]));
    __asm__ volatile(
        "addu %0,$zero,$zero\n\t"
        "lui %1,%%hi(D_801E42F8)\n\t"
        "lbu %1,%%lo(D_801E42F8)(%1)\n\t"
        "addu %2,%3,$zero"
        : "=r"(i), "=r"(bound), "=r"(store_voice)
        : "r"(voice));
    D_801E4BE6 = store_voice;
    sum = 0;

    if (bound > 0) {
        voice <<= 16;
        do {
            sum += (s16)SpuVmApplyPitchBendToVoice((s16)i, (s16)(voice >> 16), (s16)x, (s16)y, (u16)extra);
            next = i + 1;
            i = next;
            __asm__("" : "=r"(i) : "0"(i));
        } while ((s16)next < D_801E42F8);
    }

    return sum;
}

#include <sys/types.h>

#include "common.h"

extern volatile u_char D_801E42F8;
extern short g_SndCurrentSeqSep asm("D_801E4BE6");

void SpuVmVSetUp(long arg0, long arg1) asm("func_80073314");
long SpuVmApplyPitchBendToVoice(long arg0, long arg1, long arg2, long arg3, long arg4) asm("func_80075CB0");

long SpuVmApplyPitchBendByTone(long arg0, long arg1, long arg2, long arg3) asm("func_80075EB4");

long SpuVmApplyPitchBendByTone(long arg0, long arg1, long arg2, long arg3) {
    register long voice asm("$18");
    register long x asm("$21");
    register long y asm("$20");
    register long extra asm("$19");
    register long i asm("$16");
    register long sum asm("$17");
    register long bound asm("$3");
    register long store_voice asm("$2");
    register long tmp asm("$2");
    register long next asm("$2");
    long stack_pad[2];
    long call_x;
    long call_y;

    voice = arg0;
    tmp = arg1;
    call_x = (short)tmp;
    call_y = (short)arg2;
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
    g_SndCurrentSeqSep = store_voice;
    sum = 0;

    if (bound > 0) {
        voice <<= 16;
        do {
            sum += (short)SpuVmApplyPitchBendToVoice((short)i, (short)(voice >> 16), (short)x, (short)y, (u_short)extra);
            next = i + 1;
            i = next;
            __asm__("" : "=r"(i) : "0"(i));
        } while ((short)next < D_801E42F8);
    }

    return sum;
}

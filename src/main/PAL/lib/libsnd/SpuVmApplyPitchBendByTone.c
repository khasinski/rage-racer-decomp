#include <sys/types.h>

#include "common.h"

extern volatile u_char D_801E42F8;
extern short g_SndCurrentSeqSep asm("D_801E4BE6");

void SpuVmVSetUp(long arg0, long arg1) asm("func_80073314");
long SpuVmApplyPitchBendToVoice(long arg0, long arg1, long arg2, long arg3, long arg4) asm("func_80075CB0");

long SpuVmApplyPitchBendByTone(long arg0, long arg1, long arg2, long arg3) asm("func_80075EB4");

long SpuVmApplyPitchBendByTone(long arg0, long arg1, long arg2, long arg3) {
    long voice;
    long x;
    long y;
    long extra;
    long i;
    long sum;
    long bound;
    register long tmp asm("$2");
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
    i = 0;
    sum = 0;
    bound = D_801E42F8;
    __asm__ volatile("");
    tmp = voice;
    g_SndCurrentSeqSep = tmp;

    if ((short)i < bound) {
        voice <<= 16;
        do {
            sum += (short)SpuVmApplyPitchBendToVoice((short)i, (short)(voice >> 16), (short)x, (short)y, (u_short)extra);
            tmp = i + 1;
            i = tmp;
            __asm__("" : "=r"(i) : "0"(i));
        } while ((short)tmp < (bound = D_801E42F8));
    }

    return sum;
}

#include <sys/types.h>

#include "common.h"

void SsPackAdsr(u_short *in, u_short *out0, u_short *out1) asm("func_80070E84");

void SsPackAdsr(u_short *in, u_short *out0, u_short *out1) {
    u_long mask = 0xFFFF8000;
    long has8000 = ((short *)in)[6];
    /* This pin is load-bearing: removing it changes .text. */
    register long has4000 asm("$3") = ((short *)in)[8];
    u_long pred;
    u_long word1High;
    long hasWord0High;
    /* This pin is load-bearing: removing it changes .text. */
    register u_long word1 asm("$8");
    u_long word0High;
    u_long word0;
    /* These pins are load-bearing: removing any one changes .text. */
    register u_long tmp asm("$2");
    register u_long tmp2 asm("$3");

    pred = has8000 != 0;
    pred = -pred;
    word1High = pred & mask;
    hasWord0High = ((short *)in)[5];
    word1 = word1High;
    pred = hasWord0High != 0;
    pred = -pred;
    word0High = pred & mask;

    if (has4000 != 0) {
        word1 = word1High | 0x4000;
    }

    word0 = word0High | ((in[0] << 8) & 0x7F00);
    tmp = (in[1] << 4) & 0xF0;
    word0 = word0 | tmp;
    tmp2 = in[2] & 0xF;
    word0 = word0 | tmp2;

    tmp = in[3];
    tmp2 = in[4];
    *out0 = word0;

    tmp = (tmp << 6) & 0x1FC0;
    tmp = word1 | tmp;
    tmp2 = tmp2 & 0x1F;
    word1 = tmp | tmp2;
    *out1 = word1;
}

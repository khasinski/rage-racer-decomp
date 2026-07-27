#include <sys/types.h>

#include "common.h"

void SsPackAdsr(u_short *in, u_short *out0, u_short *out1) asm("func_80070E84");

void SsPackAdsr(u_short *in, u_short *out0, u_short *out1) {
    register u_long mask asm("$4") = 0xFFFF8000;
    register long has8000 asm("$2") = ((short *)in)[6];
    register long has4000 asm("$3") = ((short *)in)[8];
    register u_long pred asm("$2");
    register u_long word1High asm("$9");
    register long hasWord0High asm("$2");
    register u_long word1 asm("$8");
    register u_long word0High asm("$10");
    register u_long word0 asm("$4");
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

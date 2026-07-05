#include "common.h"

void SsPackAdsr(u16 *in, u16 *out0, u16 *out1) asm("func_80070E84");

void SsPackAdsr(u16 *in, u16 *out0, u16 *out1) {
    register u32 mask asm("$4") = 0xFFFF8000;
    register s32 has8000 asm("$2") = ((s16 *)in)[6];
    register s32 has4000 asm("$3") = ((s16 *)in)[8];
    register u32 pred asm("$2");
    register u32 word1High asm("$9");
    register s32 hasWord0High asm("$2");
    register u32 word1 asm("$8");
    register u32 word0High asm("$10");
    register u32 word0 asm("$4");
    register u32 tmp asm("$2");
    register u32 tmp2 asm("$3");

    pred = has8000 != 0;
    pred = -pred;
    word1High = pred & mask;
    hasWord0High = ((s16 *)in)[5];
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

#include "common.h"
#include "psyq/kernel.h"

s32 func_800657E4(void);

void *func_80064AA8(u8 *env, s32 x, s32 y, s32 w, s32 h) {
    register u8 *envReg asm("$17") = env;
    register s32 xReg asm("$19") = x;
    register s32 yReg asm("$20") = y;
    register s32 wReg asm("$16") = w;
    register s32 hReg asm("$18") = h;
    register s32 cmp asm("$2");
    s32 dmaState;
    s32 graphType;

    dmaState = GetDMAInterruptState();
    *(s16 *)&envReg[0x0] = xReg;
    *(s16 *)&envReg[0x2] = yReg;
    *(s16 *)&envReg[0x4] = wReg;
    *(s16 *)&envReg[0xC] = 0;
    *(s16 *)&envReg[0xE] = 0;
    *(s16 *)&envReg[0x10] = 0;
    *(s16 *)&envReg[0x12] = 0;
    envReg[0x19] = 0;
    envReg[0x1A] = 0;
    envReg[0x1B] = 0;
    envReg[0x16] = 1;
    *(s16 *)&envReg[0x6] = hReg;
    if (dmaState != 0) {
        cmp = hReg < 0x121;
    } else {
        cmp = hReg < 0x101;
    }
    envReg[0x17] = cmp;
    *(s16 *)&envReg[0x8] = xReg;
    *(s16 *)&envReg[0xA] = yReg;

    graphType = func_800657E4();
    if (graphType != 1) {
        func_800657E4();
    }

    *(s16 *)&envReg[0x14] = 10;
    envReg[0x18] = 0;
    return envReg;
}

#include "common.h"
#include "psyq/kernel.h"

extern s32 D_8009B538[];
extern s32 D_8019C864;

s32 func_8005F35C(void) {
    register s32 result asm("$16");
    register s32 ready asm("$17");
    s32 count;

    ready = 1;
    if (TestEvent(D_8009B538[0]) == ready) {
        result = 1;
    }
    if (TestEvent(D_8009B538[1]) == ready) {
        result = 2;
    }
    if (TestEvent(D_8009B538[2]) == ready) {
        result = 3;
    }
    if (TestEvent(D_8009B538[3]) == ready) {
        result = 4;
    }

    count = D_8019C864;
    D_8019C864 = count + 1;
    if (count >= 90) {
        result = 2;
    }

    return result;
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005F35C", func_8005F420);

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005F35C", func_8005F4D8);

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005F35C", func_8005F55C);

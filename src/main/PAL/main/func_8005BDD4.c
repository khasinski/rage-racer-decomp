#include "common.h"

extern s32 D_801E6CA4;

void func_8005BDD4(s32 arg0) {
    if (arg0 >= 0) {
        if (arg0 < 0x10) {
            goto done;
        }
        arg0 = 0xF;
    } else {
        arg0 = 0;
    }

done:
    {
        register s32 product asm("$3");
        register s32 value asm("$2");

        value = 0x88888889;
        product = arg0 << 7;
        __asm__ volatile(
            "mult %1,%0\n\t"
            "mfhi %0"
            : "=r"(value)
            : "r"(product), "0"(value)
            : "hi", "lo");
        value += product;
        value >>= 3;
        product >>= 31;
        value -= product;
        D_801E6CA4 = value;
    }
}

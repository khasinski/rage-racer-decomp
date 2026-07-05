#include "psyq/kernel.h"

extern void *D_8009A4CC[];

void setIntrVSync(s32 arg0, void *arg1) {
    register void **base asm("$2");
    register void **slot asm("$4");

    base = D_8009A4CC;
    slot = &base[arg0];
    if (arg1 != *slot) {
        *slot = arg1;
    }
}

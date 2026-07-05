#include "common.h"

s32 func_80063220(void) {
    asm volatile(
        "addiu $4,$0,2\n"
        "syscall");
}

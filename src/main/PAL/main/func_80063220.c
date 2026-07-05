#include "common.h"

void func_80063220(void) {
    __asm__ volatile(
        "addiu $4,$0,2\n\t"
        "syscall 0");
}

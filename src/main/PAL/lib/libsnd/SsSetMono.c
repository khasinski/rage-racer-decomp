#include "common.h"

extern s16 D_801E3FB0;

void SsSetMono(void) asm("func_80072AE0");

void SsSetMono(void) {
    D_801E3FB0 = 1;
}

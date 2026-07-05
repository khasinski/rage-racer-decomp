#include "common.h"

extern s16 D_801E3FB0;

void SsSetStereo(void) asm("func_80072AF4");

void SsSetStereo(void) {
    D_801E3FB0 = 0;
}

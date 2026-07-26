#include "common.h"

extern s32 D_8009B1B4;

void GameStartCdVolumeFade(s32 arg0) asm("func_80042CCC");
void GameStartCdVolumeFade(s32 arg0) {
    D_8009B1B4 = arg0;
    if (arg0 >= 0x1000) {
        D_8009B1B4 = 0xFFF;
    }
    if (D_8009B1B4 < -0xFFF) {
        D_8009B1B4 = -0xFFF;
    }
}

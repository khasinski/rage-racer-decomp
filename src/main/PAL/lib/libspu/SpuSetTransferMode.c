#include "psyq/spu.h"

extern s32 D_8009A714;
extern s32 D_8009AB94;

void SpuSetTransferMode(s32 arg0) {
    register s32 value asm("$2");

    value = 1;
    if (arg0 == 0) {
        value = 0;
    } else {
        if (arg0 == value) {
            value = 1;
        } else {
            value = 0;
        }
    }

    D_8009A714 = arg0;
    D_8009AB94 = value;
}

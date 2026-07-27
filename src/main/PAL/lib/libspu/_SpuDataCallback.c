#include "psyq/spu.h"

void DMACallback(s32 spec, void *callback) asm("func_8006DF94");

void _SpuDataCallback(void *callback) {
    DMACallback(4, callback);
}

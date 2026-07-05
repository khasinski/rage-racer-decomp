#include "psyq/spu.h"

extern u32 D_8009ABAC;

u32 _spu_isTransferIdle(void) {
    return D_8009ABAC == 0;
}

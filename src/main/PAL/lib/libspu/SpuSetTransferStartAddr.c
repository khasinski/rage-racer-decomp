#include "psyq/spu.h"

extern u16 D_8009AB78;

s32 SpuSetTransferStartAddr(s32 arg0) {
    D_8009AB78 = _spu_FsetRXXa(-1, arg0);
    return arg0;
}

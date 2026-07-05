#include "psyq/spu.h"

extern s32 D_8009ABAC;

void _spu_setTransferCompletionFlag(s32 arg0) {
    if (arg0 == 1) {
        D_8009ABAC = 0;
    } else {
        D_8009ABAC = 1;
    }
}

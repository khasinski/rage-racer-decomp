#include "psyq/spu.h"

extern s32 D_8009ABAC;
extern s32 D_8009ABB0;

u32 Spu_ReadFromSpu(s32 arg0, u32 arg1) {
    u32 size = arg1;

    if (0x7F000 < size) {
        size = 0x7F000;
    }

    _spu_Fw(arg0, size);

    if (D_8009ABB0 == 0) {
        D_8009ABAC = 0;
    }

    return size;
}

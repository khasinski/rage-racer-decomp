#include "common.h"


INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libsnd/SsVabTransBody", SsVabTransBody);

s16 SsVabTransCompleted(s16 immediate_flag) asm("func_8007317C");
s16 SsVabTransCompleted(s16 immediate_flag) {
    s32 SpuIsTransferCompleted(s32 wait) asm("func_8007B1EC");

    return SpuIsTransferCompleted(immediate_flag);
}

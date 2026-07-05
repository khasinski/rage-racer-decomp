#include "common.h"

extern s16 D_801E4BE6;

s32 SpuVmVSetUp(s32 arg0, s32 arg1) asm("func_80073314");
s32 SpuVmApplyPitchBendToVoice(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) asm("func_80075CB0");

s32 SsUtPitchBend(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) asm("func_80078130");

s32 SsUtPitchBend(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4) {
    s32 id;
    u16 extra;
    s32 x;
    s32 y;
    s32 ret;

    id = arg0;
    extra = arg4;
    x = (s16)arg1;
    y = (s16)arg2;

    SpuVmVSetUp(x, y);
    D_801E4BE6 = 0x21;
    ret = SpuVmApplyPitchBendToVoice((s16)id, 0x21, x, y, extra);
    return -(((u32)(ret << 16)) < 1U);
}

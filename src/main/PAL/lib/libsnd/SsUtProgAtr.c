#include "common.h"

extern u8 *D_801E4110;

s32 SpuVmVSetUp(s32 arg0, s32 arg1) asm("func_80073314");

s32 SsUtSetProgVol(s32 arg0, s32 arg1, s32 arg2) asm("func_8007701C");
s32 SsUtGetProgVol(s32 arg0, s32 arg1) asm("func_80077090");
s32 SsUtSetProgPan(s32 arg0, s32 arg1, s32 arg2) asm("func_800770E4");
s32 SsUtGetProgPan(s32 arg0, s32 arg1) asm("func_80077158");

s32 SsUtSetProgVol(s32 arg0, s32 arg1, s32 arg2) {
    s32 value;
    s32 arg0s;
    s32 index;

    value = arg2;
    arg0s = (s16)arg0;
    index = (s16)arg1;
    if (SpuVmVSetUp(arg0s, index) == 0) {
        D_801E4110[(index << 4) + 0x1] = value;
        return D_801E4110[(index << 4) + 0x1];
    }
    return -1;
}

s32 SsUtGetProgVol(s32 arg0, s32 arg1) {
    s32 arg0s;
    s32 index;

    arg0s = (s16)arg0;
    index = (s16)arg1;
    if (SpuVmVSetUp(arg0s, index) == 0) {
        return D_801E4110[(index << 4) + 0x1];
    }
    return -1;
}

s32 SsUtSetProgPan(s32 arg0, s32 arg1, s32 arg2) {
    s32 value;
    s32 arg0s;
    s32 index;

    value = arg2;
    arg0s = (s16)arg0;
    index = (s16)arg1;
    if (SpuVmVSetUp(arg0s, index) == 0) {
        D_801E4110[(index << 4) + 0x4] = value;
        return D_801E4110[(index << 4) + 0x4];
    }
    return -1;
}

s32 SsUtGetProgPan(s32 arg0, s32 arg1) {
    s32 arg0s;
    s32 index;

    arg0s = (s16)arg0;
    index = (s16)arg1;
    if (SpuVmVSetUp(arg0s, index) == 0) {
        return D_801E4110[(index << 4) + 0x4];
    }
    return -1;
}

#include <sys/types.h>

#include "common.h"

extern u_char *g_SndCurrentProgTable asm("D_801E4110");

long SpuVmVSetUp(long arg0, long arg1) asm("func_80073314");

long SsUtSetProgVol(long arg0, long arg1, long arg2) asm("func_8007701C");
long SsUtGetProgVol(long arg0, long arg1) asm("func_80077090");
long SsUtSetProgPan(long arg0, long arg1, long arg2) asm("func_800770E4");
long SsUtGetProgPan(long arg0, long arg1) asm("func_80077158");

long SsUtSetProgVol(long arg0, long arg1, long arg2) {
    long value;
    long arg0s;
    long index;

    value = arg2;
    arg0s = (short)arg0;
    index = (short)arg1;
    if (SpuVmVSetUp(arg0s, index) == 0) {
        g_SndCurrentProgTable[(index << 4) + 0x1] = value;
        return g_SndCurrentProgTable[(index << 4) + 0x1];
    }
    return -1;
}

long SsUtGetProgVol(long arg0, long arg1) {
    long arg0s;
    long index;

    arg0s = (short)arg0;
    index = (short)arg1;
    if (SpuVmVSetUp(arg0s, index) == 0) {
        return g_SndCurrentProgTable[(index << 4) + 0x1];
    }
    return -1;
}

long SsUtSetProgPan(long arg0, long arg1, long arg2) {
    long value;
    long arg0s;
    long index;

    value = arg2;
    arg0s = (short)arg0;
    index = (short)arg1;
    if (SpuVmVSetUp(arg0s, index) == 0) {
        g_SndCurrentProgTable[(index << 4) + 0x4] = value;
        return g_SndCurrentProgTable[(index << 4) + 0x4];
    }
    return -1;
}

long SsUtGetProgPan(long arg0, long arg1) {
    long arg0s;
    long index;

    arg0s = (short)arg0;
    index = (short)arg1;
    if (SpuVmVSetUp(arg0s, index) == 0) {
        return g_SndCurrentProgTable[(index << 4) + 0x4];
    }
    return -1;
}

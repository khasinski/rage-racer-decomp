#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"

extern u_char *g_SndCurrentProgTable;

long SsUtSetProgVol(long vab_id, long program, long volume) {
    long value;
    long arg0s;
    long index;

    value = volume;
    arg0s = (short)vab_id;
    index = (short)program;
    if (SpuVmVSetUp(arg0s, index) == 0) {
        g_SndCurrentProgTable[(index << 4) + 0x1] = value;
        return g_SndCurrentProgTable[(index << 4) + 0x1];
    }
    return -1;
}

long SsUtGetProgVol(long vab_id, long program) {
    long arg0s;
    long index;

    arg0s = (short)vab_id;
    index = (short)program;
    if (SpuVmVSetUp(arg0s, index) == 0) {
        return g_SndCurrentProgTable[(index << 4) + 0x1];
    }
    return -1;
}

long SsUtSetProgPan(long vab_id, long program, long pan) {
    long value;
    long arg0s;
    long index;

    value = pan;
    arg0s = (short)vab_id;
    index = (short)program;
    if (SpuVmVSetUp(arg0s, index) == 0) {
        g_SndCurrentProgTable[(index << 4) + 0x4] = value;
        return g_SndCurrentProgTable[(index << 4) + 0x4];
    }
    return -1;
}

long SsUtGetProgPan(long vab_id, long program) {
    long arg0s;
    long index;

    arg0s = (short)vab_id;
    index = (short)program;
    if (SpuVmVSetUp(arg0s, index) == 0) {
        return g_SndCurrentProgTable[(index << 4) + 0x4];
    }
    return -1;
}

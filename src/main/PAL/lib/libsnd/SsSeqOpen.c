#include <sys/types.h>

#include "common.h"

extern long g_SndSeqOpenMask;
extern char g_MsgSeqTableFull[];

long SsSeqParseHeader(long arg0, long arg1, long arg2);

long SsSeqOpen(long seq_data, long vab_id);

long SsSeqOpen(long seq_data, long vab_id) {
    long used;
    long i;
    u_char found;
    long slot;

    used = g_SndSeqOpenMask;
    if (used == -1) {
        DebugPrintf(g_MsgSeqTableFull);
        return -1;
    }

    i = 0;
    found = 0;
    do {
        if (((1 << i) & used) == 0) {
            slot = i;
            found = 1;
        }
        i++;
    } while (found == 0);

    g_SndSeqOpenMask = (1 << (short)slot) | g_SndSeqOpenMask;

    if ((short)SsSeqParseHeader((short)slot, (short)vab_id, seq_data) == -1) {
        return -1;
    }
    return (short)slot;
}

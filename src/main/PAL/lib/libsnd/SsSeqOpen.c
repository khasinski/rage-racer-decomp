#include <sys/types.h>
#include <stdio.h>

#include "common.h"

#include "psyq/snd_internal.h"

long SsSeqOpen(u_char *seqData, long vabId) {
    long used;
    long i;
    u_char found;
    long slot;

    used = g_SndSeqOpenMask;
    if (used == -1) {
        printf(g_MsgSeqTableFull);
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

    if ((short)SsSeqParseHeader((short)slot, (short)vabId, seqData) == -1) {
        return -1;
    }
    return (short)slot;
}

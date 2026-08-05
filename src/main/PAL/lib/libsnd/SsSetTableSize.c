#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"

extern u_char *g_SndSeqTable[];

void SsSetTableSize(u_char *arg0, long arg1, long arg2) {
    volatile long pad[5];
    long signedArg;
    long outer;
    long firstOffset;
    long inner;
    volatile u_char **table;
    volatile u_char **row;
    volatile u_char **slot;
    long offset;
    register long step asm("$8");
    long tmp;
    register long ff asm("$3");
    register long base76 asm("$3");
    long keepGoing;
    long limit;

    signedArg = arg1;
    g_SndSeqTableSMax = signedArg;
    signedArg <<= 16;
    signedArg >>= 16;
    g_SndSeqTableTMax = arg2;

    outer = 0;
    if (signedArg > 0) {
        tmp = arg2 << 16;
        step = tmp >> 16;
        table = (volatile u_char **)g_SndSeqTable;
        firstOffset = 0;
        do {
            tmp = firstOffset * 0xAC;
            *table = arg0 + tmp;
            table++;
            outer++;
            firstOffset += step;
        } while (outer < signedArg);
    }

    outer = 0;
    if (g_SndSeqTableSMax > 0) {
        step = 0x7F;
        row = (volatile u_char **)g_SndSeqTable;
        do {
            inner = 0;
            if (g_SndSeqTableTMax > 0) {
                slot = row;
                offset = 0;
                do {
                    *(long *)(offset + (long)*slot + 0x90) = 0;
                    ff = 0xFF;
                    *(u_char *)(offset + (long)*slot + 0x3C) = ff;
                    *(u_char *)(offset + (long)*slot + 0x0) = 0;
                    *(short *)(offset + (long)*slot + 0x3E) = 0;
                    *(short *)(offset + (long)*slot + 0x40) = 0;
                    *(long *)(offset + (long)*slot + 0x94) = 0;
                    *(long *)(offset + (long)*slot + 0x98) = 0;
                    *(short *)(offset + (long)*slot + 0x42) = 0;
                    *(long *)(offset + (long)*slot + 0xA4) = 0;
                    *(long *)(offset + (long)*slot + 0xA0) = 0;
                    *(long *)(offset + (long)*slot + 0x9C) = 0;
                    *(short *)(offset + (long)*slot + 0x44) = 0;
                    *(short *)(offset + (long)*slot + 0x74) = step;
                    *(short *)(offset + (long)*slot + 0x76) = step;
                    inner++;
                    *(short *)(offset + (long)*slot + 0x78) = step;
                    base76 = (long)*slot;
                    limit = g_SndSeqTableTMax;
                    base76 = offset + base76;
                    offset += 0xAC;
                    keepGoing = inner < limit;
                    *(short *)(base76 + 0x7A) = step;
                } while (keepGoing);
            }
            limit = g_SndSeqTableSMax;
            outer++;
            keepGoing = outer < limit;
            row++;
        } while (keepGoing);
    }
}

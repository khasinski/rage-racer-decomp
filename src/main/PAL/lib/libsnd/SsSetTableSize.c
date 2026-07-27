#include <sys/types.h>

#include "common.h"

extern u_char *D_801E79CC[];
extern short D_801E826C;
extern short D_801E8270;

void SsSetTableSize(u_char *arg0, long arg1, long arg2) asm("func_80072310");

void SsSetTableSize(u_char *arg0, long arg1, long arg2) {
    volatile long pad[5];
    register long signedArg asm("$5");
    register long outer asm("$7");
    register long firstOffset asm("$3");
    register long inner asm("$6");
    register volatile u_char **table asm("$6");
    register volatile u_char **row asm("$9");
    register volatile u_char **slot asm("$5");
    register long offset asm("$4");
    register long step asm("$8");
    register long tmp asm("$2");
    register long ff asm("$3");
    register long base76 asm("$3");
    register long keepGoing asm("$2");
    long limit;

    signedArg = arg1;
    D_801E826C = signedArg;
    signedArg <<= 16;
    signedArg >>= 16;
    D_801E8270 = arg2;

    outer = 0;
    if (signedArg > 0) {
        tmp = arg2 << 16;
        step = tmp >> 16;
        table = (volatile u_char **)D_801E79CC;
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
    if (D_801E826C > 0) {
        step = 0x7F;
        row = (volatile u_char **)D_801E79CC;
        do {
            inner = 0;
            if (D_801E8270 > 0) {
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
                    limit = D_801E8270;
                    base76 = offset + base76;
                    offset += 0xAC;
                    keepGoing = inner < limit;
                    *(short *)(base76 + 0x7A) = step;
                } while (keepGoing);
            }
            limit = D_801E826C;
            outer++;
            keepGoing = outer < limit;
            row++;
        } while (keepGoing);
    }
}

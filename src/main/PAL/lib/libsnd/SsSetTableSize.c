#include "common.h"

extern u8 *D_801E79CC[];
extern s16 D_801E826C;
extern s16 D_801E8270;

void SsSetTableSize(u8 *arg0, s32 arg1, s32 arg2) asm("func_80072310");

void SsSetTableSize(u8 *arg0, s32 arg1, s32 arg2) {
    volatile s32 pad[5];
    register s32 signedArg asm("$5");
    register s32 outer asm("$7");
    register s32 firstOffset asm("$3");
    register s32 inner asm("$6");
    register volatile u8 **table asm("$6");
    register volatile u8 **row asm("$9");
    register volatile u8 **slot asm("$5");
    register s32 offset asm("$4");
    register s32 step asm("$8");
    register s32 tmp asm("$2");
    register s32 ff asm("$3");
    register s32 base76 asm("$3");
    register s32 keepGoing asm("$2");
    s32 limit;

    signedArg = arg1;
    D_801E826C = signedArg;
    signedArg <<= 16;
    signedArg >>= 16;
    D_801E8270 = arg2;

    outer = 0;
    if (signedArg > 0) {
        tmp = arg2 << 16;
        step = tmp >> 16;
        table = (volatile u8 **)D_801E79CC;
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
        row = (volatile u8 **)D_801E79CC;
        do {
            inner = 0;
            if (D_801E8270 > 0) {
                slot = row;
                offset = 0;
                do {
                    *(s32 *)(offset + (s32)*slot + 0x90) = 0;
                    ff = 0xFF;
                    *(u8 *)(offset + (s32)*slot + 0x3C) = ff;
                    *(u8 *)(offset + (s32)*slot + 0x0) = 0;
                    *(s16 *)(offset + (s32)*slot + 0x3E) = 0;
                    *(s16 *)(offset + (s32)*slot + 0x40) = 0;
                    *(s32 *)(offset + (s32)*slot + 0x94) = 0;
                    *(s32 *)(offset + (s32)*slot + 0x98) = 0;
                    *(s16 *)(offset + (s32)*slot + 0x42) = 0;
                    *(s32 *)(offset + (s32)*slot + 0xA4) = 0;
                    *(s32 *)(offset + (s32)*slot + 0xA0) = 0;
                    *(s32 *)(offset + (s32)*slot + 0x9C) = 0;
                    *(s16 *)(offset + (s32)*slot + 0x44) = 0;
                    *(s16 *)(offset + (s32)*slot + 0x74) = step;
                    *(s16 *)(offset + (s32)*slot + 0x76) = step;
                    inner++;
                    *(s16 *)(offset + (s32)*slot + 0x78) = step;
                    base76 = (s32)*slot;
                    limit = D_801E8270;
                    base76 = offset + base76;
                    offset += 0xAC;
                    keepGoing = inner < limit;
                    *(s16 *)(base76 + 0x7A) = step;
                } while (keepGoing);
            }
            limit = D_801E826C;
            outer++;
            keepGoing = outer < limit;
            row++;
        } while (keepGoing);
    }
}

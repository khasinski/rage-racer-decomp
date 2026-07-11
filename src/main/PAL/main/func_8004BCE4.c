#include "common.h"

extern u32 D_801E6F2C[];

void func_8005D6EC(s32 arg0);

void func_8004BCE4(void) {
    register u32 *base asm("$14");
    register s32 row asm("$13");
    register s32 highStart asm("$15");
    register s32 rowOffset asm("$9");
    register u32 *lowBase asm("$8");
    register s32 pairOffset asm("$11");
    register s32 highIndex asm("$7");
    register s32 nibble asm("$4");
    register u32 lowPacked asm("$5");
    register u32 highPacked asm("$6");
    register u32 lowWord asm("$12");
    register u32 highWord asm("$10");
    register u32 shift asm("$2");
    register u32 lowNibble asm("$3");

    func_8005D6EC(8);

    base = D_801E6F2C;
    row = 0;
    highStart = 7;
    do {
        rowOffset = row << 5;
        lowBase = base;
        pairOffset = 0;
        highIndex = highStart;
        do {
            nibble = 0;
            lowPacked = 0;
            highPacked = 0;
            lowWord = *(u32 *)(rowOffset + (s32)lowBase);
            highWord = *(u32 *)(rowOffset + (s32)&base[highIndex]);
            do {
                lowPacked <<= 4;
                shift = nibble << 2;
                lowNibble = (lowWord >> shift) & 0xF;
                asm("" : "=r"(lowNibble) : "0"(lowNibble));
                highPacked = (highPacked << 4) | ((highWord >> shift) & 0xF);
                nibble++;
                lowPacked |= lowNibble;
            } while (nibble < 8);
            shift = rowOffset + (s32)lowBase;
            lowBase++;
            pairOffset += 4;
            lowNibble = highIndex << 2;
            lowNibble += (s32)base;
            lowNibble = rowOffset + lowNibble;
            *(u32 *)lowNibble = lowPacked;
            *(u32 *)shift = highPacked;
            highIndex--;
        } while (pairOffset < 0x10);
        row++;
    } while (row < 0x40);
}

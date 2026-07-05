#include "common.h"

extern u8 D_801E4BD7;
extern u8 D_801E4BDC;
extern u8 *D_801E416C;
extern u16 D_8009A58C[];

u16 SpuVmCalculateTonePitch(s32 arg0, s32 arg1) asm("func_80074A6C");

u16 SpuVmCalculateTonePitch(s32 arg0, s32 arg1) {
    register u8 *entry asm("v1");
    register s32 arg0_hold asm("t0");
    register s32 bank asm("a2");
    register s32 quotient8 asm("a1");
    register s32 sum asm("v0");
    register s32 nibble asm("a3");
    register s32 raw_delta asm("v0");
    register s32 delta asm("v1");
    register s32 quotient asm("a0");
    s32 table_index;
    s32 shift;

    entry = D_801E416C + (((D_801E4BDC + (D_801E4BD7 << 4)) << 5));
    sum = (u16)arg1 + entry[5];
    if (sum < 0) {
        arg0_hold = arg0;
        sum += 7;
    } else {
        arg0_hold = arg0;
    }

    quotient8 = sum >> 3;
    __asm__ volatile("" : "=r"(quotient8) : "0"(quotient8));
    nibble = quotient8;
    bank = 0;
    if (quotient8 >= 0x10) {
        bank = 1;
        nibble = quotient8 - 0x10;
    }

    {
        register s32 entry_value asm("v1") = *(entry + 4);
        raw_delta = arg0_hold + (0x3C - entry_value);
        raw_delta = bank + raw_delta;
    }
    raw_delta <<= 16;
    delta = raw_delta >> 16;
    quotient = delta / 12;
    __asm__ volatile("" : "=r"(quotient) : "0"(quotient));
    delta -= quotient * 12;
    table_index = (delta << 16) >> 12;
    {
        register s32 signed_nibble asm("v0") = (s16)nibble;
        table_index += signed_nibble;
    }
    shift = (s16)(quotient - 5);

    {
        register s32 value asm("v1");
        value = D_8009A58C[table_index];
        if (shift > 0) {
            value <<= shift;
        } else if (shift < 0) {
            value = (u16)value;
            value >>= -shift;
        }
        return value;
    }
}

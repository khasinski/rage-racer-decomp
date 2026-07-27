#include <sys/types.h>

#include "common.h"

extern u_char D_801E4BD2;
extern u_char D_801E4BD7;
extern u_char D_801E4BDC;
extern u_char D_801E4BE0;
extern u_char D_801E4BE1;
extern u_char *D_801E416C;
extern u_short D_8009A58C[];

u_short SpuVmCalculateCurrentPitch(void) asm("func_800749B4");
u_short SpuVmCalculateTonePitch(long arg0, long arg1) asm("func_80074A6C");

u_short SpuVmCalculateCurrentPitch(void) {
    register long delta asm("a0");
    register long nibble asm("a2");
    register long temp asm("v0");
    long quotient;

    delta = (short)(D_801E4BD2 + 0x3C - D_801E4BE0);
    quotient = delta / 12;
    {
        register long raw asm("v1");
        raw = D_801E4BE1;
        nibble = raw >> 3;
    }
    delta -= quotient * 12;
    if (nibble >= 0x10U) {
        nibble = 0xF;
    }

    temp = (delta << 16) >> 12;
    {
        register long value asm("v1");
        value = D_8009A58C[nibble + temp];
        temp = (short)(quotient - 5);
        if (temp > 0) {
            value <<= temp;
        } else if (temp < 0) {
            value = (u_short)value;
            value >>= -temp;
        }
        return value;
    }
}

u_short SpuVmCalculateTonePitch(long arg0, long arg1) {
    register u_char *entry asm("v1");
    register long arg0_hold asm("t0");
    register long bank asm("a2");
    register long quotient8 asm("a1");
    register long sum asm("v0");
    register long nibble asm("a3");
    register long raw_delta asm("v0");
    register long delta asm("v1");
    register long quotient asm("a0");
    long table_index;
    long shift;

    entry = D_801E416C + (((D_801E4BDC + (D_801E4BD7 << 4)) << 5));
    sum = (u_short)arg1 + entry[5];
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
        register long entry_value asm("v1") = *(entry + 4);
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
        register long signed_nibble asm("v0") = (short)nibble;
        table_index += signed_nibble;
    }
    shift = (short)(quotient - 5);

    {
        register long value asm("v1");
        value = D_8009A58C[table_index];
        if (shift > 0) {
            value <<= shift;
        } else if (shift < 0) {
            value = (u_short)value;
            value >>= -shift;
        }
        return value;
    }
}

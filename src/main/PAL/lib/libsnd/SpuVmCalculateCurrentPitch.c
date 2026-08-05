#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"

extern u_char *g_SndCurrentToneTable;

u_short SpuVmCalculateCurrentPitch(void) {
    register long delta asm("a0");
    register long nibble asm("a2");
    register long temp asm("v0");
    long quotient;

    delta = (short)(g_SndCurrentNote + 0x3C - g_SndCurrentToneCenter);
    quotient = delta / 12;
    {
        register long raw asm("v1");
        raw = g_SndCurrentToneShift;
        nibble = raw >> 3;
    }
    delta -= quotient * 12;
    if (nibble >= 0x10U) {
        nibble = 0xF;
    }

    temp = (delta << 16) >> 12;
    {
        register long value asm("v1");
        value = g_SndPitchTable[nibble + temp];
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
    u_char *entry;
    long arg0_hold;
    long bank;
    long quotient8;
    long sum;
    register long nibble asm("a3");
    register long raw_delta asm("v0");
    long delta;
    long quotient;
    long table_index;
    long shift;

    entry = g_SndCurrentToneTable + (((g_SndCurrentTone + (g_SndCurrentProgActual << 4)) << 5));
    sum = (u_short)arg1 + entry[5];
    if (sum < 0) {
        arg0_hold = arg0;
        sum += 7;
    } else {
        arg0_hold = arg0;
    }

    quotient8 = sum >> 3;
    nibble = quotient8;
    bank = 0;
    if (quotient8 >= 0x10) {
        bank = 1;
        nibble = quotient8 - 0x10;
    }

    {
        long entry_value = *(entry + 4);
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
        value = g_SndPitchTable[table_index];
        if (shift > 0) {
            value <<= shift;
        } else if (shift < 0) {
            value = (u_short)value;
            value >>= -shift;
        }
        return value;
    }
}

#include "common.h"

extern u8 D_801E4BD2;
extern u8 D_801E4BE0;
extern u8 D_801E4BE1;
extern u16 D_8009A58C[];

u16 SpuVmCalculateCurrentPitch(void) asm("func_800749B4");

u16 SpuVmCalculateCurrentPitch(void) {
    register s32 delta asm("a0");
    register s32 nibble asm("a2");
    register s32 temp asm("v0");
    s32 quotient;

    delta = (s16)(D_801E4BD2 + 0x3C - D_801E4BE0);
    quotient = delta / 12;
    {
        register s32 raw asm("v1");
        raw = D_801E4BE1;
        nibble = raw >> 3;
    }
    delta -= quotient * 12;
    if (nibble >= 0x10U) {
        nibble = 0xF;
    }

    temp = (delta << 16) >> 12;
    {
        register s32 value asm("v1");
        value = D_8009A58C[nibble + temp];
        temp = (s16)(quotient - 5);
        if (temp > 0) {
            value <<= temp;
        } else if (temp < 0) {
            value = (u16)value;
            value >>= -temp;
        }
        return value;
    }
}

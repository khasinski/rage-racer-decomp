#include "common.h"
#include "game/cd.h"

extern u8 D_80082FD9[];

s32 LibcToLower(s32 arg0) asm("func_80063D6C");
s32 LibcToLower(s32 arg0) {
    u8 value = arg0;

    if (D_80082FD9[value & 0xFF] & 1) {
        value = arg0 + 0x20;
    }

    return value & 0xFF;
}

u32 func_80063D9C[20] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090038,
    0,
    0x240A00A0,
    0x01400008,
    0x240900AB,
    0,
    0x240A00A0,
    0x01400008,
    0x240900AC,
    0,
    0x240A00B0,
    0x01400008,
    0x2409004A,
    0,
    0x240A00B0,
    0x01400008,
    0x2409004B,
    0,
};

void func_80063E24(s32 arg0, s32 arg1, s32 arg2);
void func_80063E34(s32 arg0);

s32 GameIssueCdCommand3F(s32 arg0) {
    func_80063E34(arg0);
    func_80063E24(arg0, 0x3F, 0);
}

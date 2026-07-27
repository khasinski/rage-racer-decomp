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

void _card_write(s32 arg0, s32 arg1, s32 arg2) asm("func_80063E24");
void _new_card(s32 arg0) asm("func_80063E34");

s32 _card_clear(s32 arg0) {
    _new_card(arg0);
    _card_write(arg0, 0x3F, 0);
}

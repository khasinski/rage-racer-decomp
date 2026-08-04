#include <sys/types.h>

#include "common.h"
#include "game/cd.h"
#include "game/memcard.h"

extern u_char g_LibcCtype[] asm("D_80082FD9");

long LibcToLower(long arg0);
long LibcToLower(long arg0) {
    u_char value = arg0;

    if (g_LibcCtype[(u8)value] & 1) {
        value = arg0 + 0x20;
    }

    return (u8)value;
}

u_long g_BiosCallStubs[20] asm("func_80063D9C") __attribute__((section(".text"))) = {
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

void _card_write(long arg0, long arg1, long arg2);
void _new_card(long arg0) asm("func_80063E34");

/* libcard, but declared in game/memcard.h, so it keeps common.h's typedefs. */
s32 _card_clear(s32 arg0) {
    _new_card(arg0);
    _card_write(arg0, 0x3F, 0);
}

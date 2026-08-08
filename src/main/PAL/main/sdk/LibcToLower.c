#include <sys/types.h>

#include "common.h"
#include "game/memcard.h"

extern u_char g_LibcCtype[];

long LibcToLower(long ch) {
    u_char value = ch;

    if (g_LibcCtype[(u8)value] & 1) {
        value = ch + 0x20;
    }

    return (u8)value;
}

u_long g_BiosCallStubs[20] __attribute__((section(".text"))) = {
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

void _card_write(long port, long block, long src);
void _new_card(long port);

/* libcard, but declared in game/memcard.h, so it keeps common.h's typedefs. */
s32 _card_clear(s32 chan) {
    _new_card(chan);
    _card_write(chan, 0x3F, 0);
}

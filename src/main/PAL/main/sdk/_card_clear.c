#include "game/memcard.h"

s32 _card_clear(s32 chan) {
    _new_card(chan);
    _card_write(chan, 0x3F, 0);
}

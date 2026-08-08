#include "common.h"
#include "game/audio.h"
#include "game/cd.h"
#include "game/memcard.h"
#include "game/sound.h"

void StartMemoryCardEvents(void) {
    OpenMemoryCardEvents();
    EnableMemoryCardEvents();
}

void StopMemoryCardEvents(void) {
    DisableMemoryCardEvents();
    CloseMemoryCardEvents();
}

void _card_load(s32 port);
void CardReadAndSetMode(s32 param) {
    ClearMemoryCardSwEvents(param);
    while (_card_clear((u8)param) == 0) {}
    WaitMemoryCardSwEvent();
    ClearMemoryCardHwEvents();
    _card_load((u8)param);
    WaitMemoryCardHwEvent();
}

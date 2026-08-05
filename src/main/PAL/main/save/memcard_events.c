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

void _card_load(s32 arg0);
void CardReadAndSetMode(s32 arg0) {
    ClearMemoryCardSwEvents(arg0);
    while (_card_clear((u8)arg0) == 0) {}
    WaitMemoryCardSwEvent();
    ClearMemoryCardHwEvents();
    _card_load((u8)arg0);
    WaitMemoryCardHwEvent();
}

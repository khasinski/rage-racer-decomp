#include "common.h"
#include "game/audio.h"
#include "game/cd.h"
#include "game/memcard.h"

extern s16 g_SeqHandle;
extern s16 g_SeqVolume;

void StartMemoryCardEvents(void) {
    OpenMemoryCardEvents();
    EnableMemoryCardEvents();
}

void StopMemoryCardEvents(void) {
    DisableMemoryCardEvents();
    CloseMemoryCardEvents();
}

void ClearMemoryCardSwEvents();
s32 _card_clear(s32 arg0);
void ClearMemoryCardHwEvents();
void _card_load(s32 arg0);
void CardReadAndSetMode(s32 arg0);
void CardReadAndSetMode(s32 arg0) {
    ClearMemoryCardSwEvents(arg0);
    while (_card_clear((u8)arg0) == 0) {}
    WaitMemoryCardSwEvent();
    ClearMemoryCardHwEvents();
    _card_load((u8)arg0);
    WaitMemoryCardHwEvent();
}

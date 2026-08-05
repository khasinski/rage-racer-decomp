#include "common.h"
#include "game/memcard.h"
void ClearMemoryCardHwEvents();
void _card_info(s32 arg0);
void CardSeekParam(s32 arg0) {
    ClearMemoryCardHwEvents(arg0);
    _card_info((u8)arg0);
    WaitMemoryCardHwEvent();
}

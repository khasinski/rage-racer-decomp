#include "common.h"
#include "game/memcard.h"
void func_8005F2AC(s32 arg0);
void _card_info(s32 arg0);
void CardSeekParam(s32 arg0);
void CardSeekParam(s32 arg0) {
    func_8005F2AC(arg0);
    _card_info((u8)arg0);
    WaitMemoryCardHwEvent();
}

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

void func_8005F304(s32 arg0);
s32 _card_clear(s32 arg0);
void func_8005F55C(void);
void func_8005F2AC(void);
void _card_load(s32 arg0);
void func_8005F4D8(void);
void CardReadAndSetMode(s32 arg0);
void CardReadAndSetMode(s32 arg0) {
    func_8005F304(arg0);
    while (_card_clear((u8)arg0) == 0) {}
    func_8005F55C();
    func_8005F2AC();
    _card_load((u8)arg0);
    func_8005F4D8();
}
